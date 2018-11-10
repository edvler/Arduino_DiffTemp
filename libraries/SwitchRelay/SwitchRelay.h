#ifndef SwitchRelay_h
#define SwitchRelay_h

#include <Arduino.h>


/**
* Switchrelay class.
* Diese Klasse stellt ein Relay dar. 
*
* @author Matthias Maderer
* @version 1.1.7
*/
class SwitchRelay {
public:
  SwitchRelay();

  /**
  * @param name Name des Relays zur späteren Verwendung im Webinterface etc.
  * @param pin PIN an welchem das Relay angeschlossen ist
  * @param highactive Wann schaltet das Relay durch? Wenn der Port des Arduino auf HIGH oder LOW steht.
  * @param arr_id Welches var_array Element ist für dieses Relay vorgesehen? Das var_array ist in der Automatic Class vorhanden und wird dort ausgwertet.
  */
  SwitchRelay(char * name, int pin, boolean highactive, byte arr_id);

  /**
  * @param to_state Status in welches das Relay gesetzt werden soll. (0=off and 1=on, 3=off overridden, 4=on overridden, 5=override deaktivieren)
  */
  void set(byte to_state);

  /**
  * @return boolean: Ist das Relay an oder aus? (1=an, 0=aus)
  */
  boolean getStatus();

  /**
  * @return char pointer: Name des Relays abfragen
  */
  char * getName();
  
  /**
  * Gibt die Nummer des var_array's zurück
  * @return byte: Welches var_array Element ist für dieses Relay vorgesehen? Das var_array speichert Werte welche in der automatic Funktion ausgewertet werden.
  */
  byte getVarArrayID();
  
  
  /**
  * Gibt den aktuellen Modus zurück
  * @return boolean: 0=Automatik aus, 1=Automatik an
  */  
  boolean getModus();

  /**
  * Gibt den Zeitpunkt der letzten Aenderung zurück
  * @return long: Aenderung vor X Minuten
  */  
  long getLastActionInMinutes();

  /**
  * Gibt die Anzahl der Schaltvorgänge zurück
  * @return long: Schaltvorgänge
  */  
  int getOnOffCount();

  /**
  * Gibt die Anzahl der Flappings zurueck
  * @return int: Schaltvorgänge
  */  
  int getFlapCount();
  
private:
  int relayPIN; ///< An welchem Pin ist das Relay angeschlossen
  boolean relayHighActive; ///< Schaltet das Relay wenn der Arduino PIN HIGH oder LOW ist?
  boolean status; ///< Aktueller Realystatus
  boolean overridden; ///< Status Overriden
  const static int RELAYS_NAME_LAENGE=16; ///< Mögliche Länge des Raum Namens. Bei Angabe von 8 sind 7 Zeichen möglich, da der \0 Terminator beachtet werden muss.
  char relay_name[RELAYS_NAME_LAENGE]; ///< Name des Relays
  byte var_array_id;
  long last_action;
  int on_off_count;
  const static int FLAP_DETECTION_MILLIS=2000;
  const static int FLAP_SLEEP_TIME=60000;
  int flap_counter;
  
  
  /**
  Diese Funktion schaltet das Relay ein. Es beachtet dabei die highactive Variable.
  */
  void setOn();

  /**
  Diese Funktion schaltet das Relay aus. Es beachtet dabei die highactive Variable.
  */
  void setOff();
};

#endif


