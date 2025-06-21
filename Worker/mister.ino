void setup_mister(int mister_pin){
  pinMode(mister_pin, OUTPUT);
}

void activate_mister(int mister_pin) {
  digitalWrite(mister_pin, HIGH);
}

void deactivate_mister(int mister_pin) {
  digitalWrite(mister_pin, LOW);
}
