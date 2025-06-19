#include "timer.h"

NonBlockingTimer::NonBlockingTimer(unsigned long interval_ms) {
  interval = interval_ms;
  lastTime = millis();
}

void NonBlockingTimer::reset() {
  lastTime = millis();
}

void NonBlockingTimer::set_interval(unsigned long ms) {
  interval = ms;
}

bool NonBlockingTimer::is_ready() {
  unsigned long current = millis();
  if (current - lastTime >= interval) {
    lastTime = current;  // reset for next interval
    return true;
  }
  return false;
}
