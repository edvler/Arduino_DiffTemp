/*
Lässt eine LED in einem bestimmten interval blinken
 */

#include <Heartbeat.h>

//Klasse einbinden
Heartbeat * h;

//PIM definieren
int led = 13;


void setup() {                
  /*
  Heartbeat intialisieren
  Erster Parameter ist der PIN,
  Zweiter Parameter ist der Blink interval in ms (1000ms = 1sekunde)!
  
  In diesem Beispiel blinkt die LED alle 1000ms
  */
  h = new Heartbeat(led,1000);
}

void loop() {
  // blink Funktion in der Loop aufrufen..
  h->blink();

}
