#include "Arduino.h"
#include "SwitchRelay.h"


SwitchRelay::SwitchRelay(char * name, int pin, boolean highactive, byte arr_id) {
  //Variablen intialisieren
  relayPIN = pin;
  relayHighActive = highactive;
  status = true;
  overridden = false;  
  
  var_array_id = arr_id;
  
  //Pin auf Output setzten
  pinMode(relayPIN, OUTPUT);
  
  // Relay ausschalten!
  setOff();

  // Relay Name abspeichern! Wichtig ist der String Terminator \0 am Ende!
  for (int n=0;n < RELAYS_NAME_LAENGE;n++) {
    relay_name[n]=name[n];  
  }
  
  last_action = 0;
  on_off_count = 0;
  flap_counter = 0;
}

byte SwitchRelay::getVarArrayID() {
 return var_array_id;
}


boolean SwitchRelay::getStatus() {
  //Ist das Relay an/aus?	
  return status;
}

boolean SwitchRelay::getModus() {
 //Ist der Override Modus aktiv?
 if (overridden == false) {
  return 1;
 } else {
  return 0; 
 }
}

char* SwitchRelay::getName() {
  // Name als Char Array zurückgeben!
  return relay_name;
}

void SwitchRelay::setOn() {
    // Relay einschalten
    if (status == false) {
	   if(millis()-last_action<FLAP_DETECTION_MILLIS) {
		   flap_counter++;
		   delay(FLAP_SLEEP_TIME);
	   }		
		
      digitalWrite(relayPIN,relayHighActive);
	  Serial.print("Relay on: ");
	  Serial.println(relay_name);
      status = true;
	  last_action = millis();
	  on_off_count++;
	}
}

void SwitchRelay::setOff() {
    // Relay ausschalten
    if (status == true) {  
	   if(millis()-last_action<FLAP_DETECTION_MILLIS) {
		   flap_counter++;
		   delay(FLAP_SLEEP_TIME);
	   }
	
      // ~ nimmt das Gegenteil dessen, was in der Variable relayHighActive gespeichert ist.
      digitalWrite(relayPIN,~relayHighActive);
	  Serial.print("Relay off: ");
	  Serial.println(relay_name);
	  status = false;
	  last_action = millis();
	  on_off_count++;
	}
}

long SwitchRelay::getLastActionInMinutes() {
	return (millis()-last_action)/1000/60;
}

int SwitchRelay::getOnOffCount() {
	return on_off_count;
}

int SwitchRelay::getFlapCount() {
	return flap_counter;
}


void SwitchRelay::set(byte to_state) {
  // Relay ist aus
  if (to_state == 0 ) {
    // Status des Relays nur ändern wenn nicht per Override ein Status gesetzt wurde
    if(overridden == false) {
      setOff();
    }
  }
  // Relay ist an
  if (to_state == 1 ) {
    // Status des Relays nur ändern wenn nicht per Override ein Status gesetzt wurde
    if(overridden == false) {
      setOn();
    }
  }
  // Relay ist an (override!! to_state=0 und to_state=1 werden dann ignoriert!
  if (to_state == 3 ) {
    setOff();
    overridden = true;
  }
  // Relay ist aus (override!! to_state=0 und to_state=1 werden dann ignoriert!
  if (to_state == 4) {
	setOn();
    overridden = true;    
  }
  // Override ausschalten. Ab dann kann wieder mit to_state=0 und to_state=1 gearbeitet werden
  if (to_state == 5) {
    overridden = false;
  } 
}



