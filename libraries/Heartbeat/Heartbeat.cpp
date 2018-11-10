#include "Arduino.h"
#include "Heartbeat.h"

Heartbeat::Heartbeat(int heartbeat_pin, int blink_interval) {
  state = LOW;
  previousMillis = 0;
  interval = blink_interval;
  pin = heartbeat_pin;
  pinMode(pin, OUTPUT);
}

void Heartbeat::blink() {
  long currentMillis = millis(); // aktuelle Zeit in Variable speichern

  if(currentMillis - previousMillis > interval) { //prüfen ob Interval erreich
    previousMillis = currentMillis; // Zeitstempel speichern
    
    // Wenn der State auf HIGH ist setzte ihn auf LOW und umgekehrt
    if (state == LOW) {
      digitalWrite(pin, HIGH);
      state = HIGH;
    }
    else {
      digitalWrite(pin, LOW);
      state = LOW;
    }
  }  
}

