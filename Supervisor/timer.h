#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class NonBlockingTimer {
  public:
    NonBlockingTimer(unsigned long interval_ms);

    void reset(); // Reset the timer
    void set_interval(unsigned long ms); // Update interval
    bool is_ready(); // Returns true if interval has passed

  private:
    unsigned long interval;
    unsigned long lastTime;
};

#endif
