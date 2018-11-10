#ifndef Heartbeat_h
#define Heartbeat_h

#include "Arduino.h"

/**
* Hearbeat class.
* Lässt eine LED in einem angegebenen Interval Blinken.
* Nützlich für einen Heartbeat.
* 
* @author Matthias Maderer
* @version 1.0.2
*/
class Heartbeat {
public:
  /**
  * @param heartbeat_pin PIN mit der angeschlossenen LED 
  * @param blink_interval Zeitintervall in Millisekunden
  */
  Heartbeat(int heartbeat_pin, int blink_interval);
  
  /** 
  * Prüft ob der Interval überschritten ist und wechsel den PIN von HIGH auf LOW und umgekehrt
  */
  void blink();

private:
  byte pin; ///< PIN auf dem Arduino Board
  int interval; ///< Interval in Millisekunden
  byte state; ///< Aktueller State des PIN's
  long previousMillis; ///< Letzer Zeitstempel bei welchem der Status gewechselt wurde
  
};

#endif




