// Basic functions
function basic_widget_ini() { //Initiate a widget instance, so you can add elements onto it
    let widget = new ListWidget();
    widget.backgroundColor = new Color('#1e1e1e');
    widget.setPadding(16, 18, 16, 5); //The original setting is widget.setPadding(16, 16, 16, 16); but the current numbers solve the centering issue. You can later try to use the WidgetStack to center the plotting area

    return widget;
}

function capitalize_str(input_str) {
    if (typeof input_str !== 'string' || input_str.length === 0) {
        return ''; // Handle non-string input or empty strings
    } else {
        let output_str = input_str.charAt(0).toUpperCase() + input_str.slice(1).toLowerCase();
        return output_str
    }
}

function show_widget(widget) { //Show the widget
    if (config.runsInWidget) {
        Script.setWidget(widget);
    } else {
        widget.presentLarge();
    }
}

function alert_in_widget(alert_title, alert_msg, alert_detail = '') {//Display an alert in the widget since widget doesn't support real alerts
    let widget = basic_widget_ini();

    let title = widget.addText(alert_title);
    if (alert_title === 'Error' || alert_title === 'Missing API key') {
        title.textColor = Color.red();
    } else {
        title.textColor = Color.lightGray();
    }
    title.font = Font.mediumSystemFont(14);

    let widget_msg = widget.addText(alert_msg);
    let widget_msg_detail = widget.addText(alert_detail);

    if (alert_title === 'Error' || alert_title === 'Missing API key') {
        for (let msg of [widget_msg, widget_msg_detail]) {
            msg.textColor = Color.red();
            msg.font = Font.boldSystemFont(28);
        }
    }

    show_widget(widget);
    Script.complete();
}

function auto_refresh_widget(input_widget, minute_int) { //Automatically schedule a widget for refresh. ChatGpt said that minute_int >= 5, so the quickest is to update every 5 min
//How the refresh work is quite intersting in IOS. The property indicates when the widget can be refreshed again. The widget will not be refreshed before the date have been reached. It is not guaranteed that the widget will refresh at exactly the specified date.
//The refresh rate of a widget is partly up to iOS/iPadOS. For example, a widget may not refresh if the device is low on battery or the user is rarely looking at the widget.

    input_widget.refreshAfterDate = new Date(Date.now() + minute_int * 60 * 1000);
}



// Worker functions
function credentials_checking() { // Check if Scriptable's Keychain has my_thingspeak_channel_id and my_thingspeak_read_api_key
    let credentials = ["my_thingspeak_channel_id", "my_thingspeak_read_api_key"];
    let full_credentials_flag = true;
    for (let credential of credentials) {
        if (!Keychain.contains(credential)) {
            alert_in_widget(`Missing ${credential}`, `You're missing ${credential} to make the API call!`);
            credential = false;
        } else {
        }
    }
    return full_credentials_flag;
}

function handle_api_error(e) { //Handle API calling errors
    alert_in_widget('Error', 'API call failed', e);
}

function current_time_str() { //Create the time str for the current time in the format of "05/20/2025, 01:47:33 PM"
    let now = new Date();

    let time_str = now.toLocaleString(
        [],
        {
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit'
        }
    )

    return time_str;
}

async function obtain_thingspeak_data(channel_id, read_api_key, last_n_results) { // Obtain the whole data from ThingSpeak
    const url = `https://api.thingspeak.com/channels/${channel_id}/feeds.json?api_key=${read_api_key}&results=${last_n_results}`;
    let req = new Request(url);

    // Load data
    let response;
    try {
        response = await req.loadJSON();
    } catch (e) {
        handle_api_error(e);
        return;
    }

    // response is in the structure below
    // {
    //     "channel": {
    //         "id": id,
    //         "name": "name",
    //         "latitude": "0.0",
    //         "longitude": "0.0",
    //         "field1": "field1_name_str",
    //         "field2": "field2_name_str",
    //         "field3": "field3_name_str",
    //         "created_at": "create_timestamp",
    //         "updated_at": "update_timestamp",
    //         "last_entry_id": last_entry_id
    //     },
    //     "feeds": [
    //     {
    //         "created_at": "create_timestamp",
    //         "entry_id": entry_id,
    //         "field1": "field1_value",
    //         "field2": "field2_value",
    //         "field3": "field3_value"
    //     }
    //     ]
    // }

    let historical_data_list = response["feeds"]

    return historical_data_list; // historical_data_list is a list of dict where each item is a dict for all the fields' data entries at a given timestamp
}

function translate_field_name(common_name) {
    const thingspeak_fields_dict = { //This is to make the code more readable
        'temperature': 'field1',
        'humidity': 'field2',
        'light': 'field3'
    };

    return thingspeak_fields_dict[common_name];
}

function data_extractor(historical_data_list, field_name) {
    let dates = historical_data_list.map(p => new Date(p["created_at"]));
    let values = historical_data_list.map(p => p[field_name]);
    let current_value = Number(values[values.length - 1]);

    return {dates, values, current_value};
}

function day_or_night(historical_data_list, day_night_light_threshold) { // Use the last light reading and the given day_night_light_threshold to determine whether it's day or night currently
    let light_readings_dict = data_extractor(historical_data_list, translate_field_name('light'));
    let light_readings = light_readings_dict["values"];
    let last_light_reading = light_readings[light_readings.length - 1];
    if (last_light_reading >= day_night_light_threshold) {
        return "day";
    } else {
        return "night";
    }
}

function determine_ideal_humidity_ranges(historical_data_list, day_night_light_threshold) {
    let time_flag = day_or_night(historical_data_list, day_night_light_threshold);
    let humidity_ideal_min;
    let humidity_ideal_max;

    if (time_flag === "day") { // Determine the min and max for humidity. These values should agree with Worker Arduino
        humidity_ideal_min = 85;
        humidity_ideal_max = 90;
    } else if (time_flag === "night") {
        humidity_ideal_min = 90;
        humidity_ideal_max = 95;
    } else {
        console.log("Something went wrong with day_or_night()");

        return;
    }

    return {humidity_ideal_min, humidity_ideal_max};
}

function highlight_dates(dates, segments_num, start_index = 0) {
    const N = dates.length;

    if (segments_num < 1) {
        throw new Error("segments_num must be at least 1.");
    }

    if (start_index >= N) {
        throw new Error("start_index must be within the length of the dates array.");
    }

    const segment_size = Math.floor(N / segments_num);

    if (segment_size <= start_index) {
        throw new Error("segment size must be greater than start_index to avoid infinite loop or overlap.");
    }

    let highlighted_dates = [];

    for (let i = 0; ; i++) {
        let index = start_index + i * segment_size;
        if (index >= N) break;
        highlighted_dates.push(dates[index]);
    }

    return highlighted_dates;
}

// Display functions
function display_time(input_widget) { // Display the time on widget
    let time_str = current_time_str();

    let title = input_widget.addText('Updated at' + ' ' + time_str);
    title.textColor = Color.lightGray();
    title.font = Font.mediumSystemFont(14);

    return;
}

function display_data_general_settings(input_widget, data_field_name) {
    // Determine the unit to be used in the plot title
    const unit_map = {
        "temperature": "℃",
        "humidity": "%"
    };

    let data_field_unit = unit_map[data_field_name];
    if (!data_field_unit) throw new Error(`Unknown data_field_name: ${data_field_name}`);

    let title = input_widget.addText(`${capitalize_str(data_field_name)} (${data_field_unit})`);
    title.textColor = Color.lightGray();
    title.font = Font.mediumSystemFont(16);


    // Define padding
    let paddingTop = 20;
    let paddingBottom = 20;
    let paddingRight = 60;

    // Set up drawing context
    let plotWidth = 990;
    let plotHeight = 400;
    let width = plotWidth + paddingRight;
    let height = plotHeight + paddingTop + paddingBottom;

    let ctx = new DrawContext();
    ctx.size = new Size(width, height);
    ctx.opaque = false;
    ctx.setTextAlignedCenter();
    ctx.setFont(Font.mediumSystemFont(12));

    // Make the background first, so you can see the things below
    ctx.setFillColor(new Color('#1e1e1e'));
    ctx.fillRect(new Rect(0, 0, width, height));

    return {ctx, paddingTop, plotWidth, plotHeight};
}

function draw_horizontal_line_with_label_solid(ctx, y, plotWidth, color) {
    // Draw the line
    ctx.setStrokeColor(color);

    let path = new Path();
    path.move(new Point(0, y));
    path.addLine(new Point(plotWidth, y));
    ctx.addPath(path);
    ctx.strokePath();

    // Add text label to the right of the line
    ctx.setTextColor(color);
}

function draw_horizontal_line_with_label_dashed(ctx, y, plotWidth, dashLength, gapLength, color) {
    // Draw the line
    ctx.setStrokeColor(color);

    for (let x = 0; x < plotWidth; x += dashLength + gapLength) {
        let path = new Path();
        path.move(new Point(x, y));
        path.addLine(new Point(Math.min(x + dashLength, plotWidth), y));
        ctx.addPath(path);
        ctx.strokePath();
    }

    // Add text label to the right of the line
    ctx.setTextColor(color);
}

function draw_horizontal_line_with_label(ctx, value, current_value, min, max, plotWidth, plotHeight, dashLength, gapLength, color, lineWidth, label, paddingTop) { //Worker function to draw dashed horizontal lines for FGI = 0, 25, 50, 75, and 100
    // Calculate the y-coordinate based on the value
    let y = paddingTop + ((max - value) / (max - min)) * plotHeight;

    // Set stroke properties
    ctx.setLineWidth(lineWidth);


    if (value === min || value === max) {
        draw_horizontal_line_with_label_solid(ctx, y, plotWidth, Color.white());
    } else if (value === current_value) { // JS needs a special way of comparing floats
        draw_horizontal_line_with_label_dashed(ctx, y, plotWidth, dashLength, gapLength, new Color('00A5FA'));
    } else { // These are the ideal values
        draw_horizontal_line_with_label_dashed(ctx, y, plotWidth, dashLength, gapLength, Color.green());
    }

    ctx.setFont(Font.mediumSystemFont(20));
    ctx.setTextAlignedLeft(); // Align text to the left inside the text rectangle

    let paddingRight = 0; // Padding from the right edge
    let textWidth = 60;
    let textHeight = 40;
    let textX = plotWidth + paddingRight;
    let textY = y - textHeight / 2;

    let textRect = new Rect(textX + 5, textY + 7, textWidth, textHeight);
    ctx.drawTextInRect(label, textRect);
}

function draw_vertical_lines_with_labels(ctx, highlighted_dates, all_dates, plotWidth, plotHeight, paddingTop, color) {
    const N = all_dates.length;

    highlighted_dates.forEach((date, i) => {
        let index = all_dates.indexOf(date); // Get actual position in full data
        if (index === -1) return; // Skip if date is not in array

        let x = index / (N - 1) * plotWidth;

        // Draw vertical line
        let path = new Path();
        path.move(new Point(x, paddingTop));
        path.addLine(new Point(x, paddingTop + plotHeight));
        ctx.addPath(path);
        ctx.setStrokeColor(color);
        ctx.setLineWidth(2);
        ctx.strokePath();

        // Draw label below plot
        let label = `${date.getHours().toString().padStart(2, "0")}:${date.getMinutes().toString().padStart(2, "0")}:${date.getSeconds().toString().padStart(2, "0")}`;
        // .toString().padStart(2, "0") adds the leading 0 so like if your date.getMinutes() returns 0 (since this function returns a number rather than a string), the display of the time should be "0"

        ctx.setTextColor(color);
        ctx.setFont(Font.mediumSystemFont(20));
        ctx.setTextAlignedCenter();
        ctx.drawTextInRect(
            label,
            new Rect(x - 50, paddingTop + plotHeight, 100, 20)
        );
    });
}

function draw_data_line(ctx, values, paddingTop, plotWidth, plotHeight, min, max) {
    // Create path for the line chart
    let path = new Path();
    values.forEach((val, i) => {
        let x = i / (values.length - 1) * plotWidth;
        let y = paddingTop + ((max - val) / (max - min)) * plotHeight;
        if (i === 0) {
            path.move(new Point(x, y));
        } else {
            path.addLine(new Point(x, y));
        }
    });

    // Draw the path
    ctx.addPath(path);
    ctx.setStrokeColor(new Color('00A5FA')); // It's a brighter blue compared to what CNN used for the FGI timeline
    ctx.setLineWidth(2);
    ctx.strokePath();

    return;
}

function add_ctx_img_to_widget(ctx, input_widget) {
    // Create widget
    let img = ctx.getImage();
    input_widget.addImage(img);

    return;
}

function display_data_worker(input_widget, historical_data_list, data_field_name, plot_min, plot_max, data_ideal_min, data_ideal_max) {
    let {
        ctx,
        paddingTop,
        plotWidth,
        plotHeight
    } = display_data_general_settings(input_widget, data_field_name);

    // Extract data
    let {dates, values, current_value} = data_extractor(historical_data_list, translate_field_name(data_field_name));

    [plot_min, plot_max, current_value, data_ideal_min, data_ideal_max].forEach(value => {
        draw_horizontal_line_with_label(ctx, value, current_value, plot_min, plot_max, plotWidth, plotHeight, 5, 3, Color.green(), 2, value.toString(), paddingTop);
    });

    // Draw vertical lines for the dates that split the dates into several pieces
    let segments_num = 4;
    let start_index = 30;
    let highlighted_dates = highlight_dates(dates, segments_num, start_index);
    draw_vertical_lines_with_labels(ctx, highlighted_dates, dates, plotWidth, plotHeight, paddingTop, Color.white());

    draw_data_line(ctx, values, paddingTop, plotWidth, plotHeight, plot_min, plot_max);

    add_ctx_img_to_widget(ctx, input_widget);

    return;
}

function display_data(historical_data_list, day_night_light_threshold) {
    let widget = basic_widget_ini();

    display_time(widget);

    // Display humidity data
    let {
        humidity_ideal_min,
        humidity_ideal_max
    } = determine_ideal_humidity_ranges(historical_data_list, day_night_light_threshold); // Day and night have different ideal humidity ranges

    display_data_worker(widget, historical_data_list, "humidity", 80, 100, humidity_ideal_min, humidity_ideal_max);

    widget.addSpacer(10);

    // Display temperature data
    display_data_worker(widget, historical_data_list, "temperature", 21, 28, 23.9, 25);

    auto_refresh_widget(widget, 5); //Try to refresh the widget every 5 min

    show_widget(widget);
}

// Main code
async function main() {
    const last_n_results = 300; // Display the last N data entries
    const day_night_light_threshold = 500; // This should agree on the Worker Arduino's LDR's light threshold on determine whether it's day time or nighttime

    let full_credentials_flag = credentials_checking();

    if (full_credentials_flag) {
        let thingspeak_channel_id = Keychain.get("my_thingspeak_channel_id");
        let thingspeak_read_api_key = Keychain.get("my_thingspeak_read_api_key");

        let historical_data_list = await obtain_thingspeak_data(thingspeak_channel_id, thingspeak_read_api_key, last_n_results); // Obtain ThingSpeak data

        display_data(historical_data_list, day_night_light_threshold); // Display the data
    }

    Script.complete();
}

await main();

