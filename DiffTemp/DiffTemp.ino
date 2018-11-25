

//##############################################
//
// Definitionen
//
//##############################################
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Angabe der erforderlichen Pins


#include <EEPROM.h>

//EEPROM Position
int eeAdresse = 2;


//Struct zum speichern der Einschaltdifferenz und der Hysterese
struct eeWerte {
  float einschaltDifferenz;
  float hysterese;
};

eeWerte ep;

//##############################################
//
// Taster
//
//##############################################

// erstellen einiger Variablen
int Taster = 0;
int Analogwert = 0;
#define Tasterrechts 0
#define Tasteroben 1
#define Tasterunten 2
#define Tasterlinks 3
#define Tasterselect 4
#define KeinTaster 5


// Ab hier wird ein neuer Programmblock mit dem Namen "Tasterstatus" erstellt. Hier wird ausschließlich geprüft, welcher Taster gedrückt ist.
int Tasterstatus()
{
  Analogwert = analogRead(A0); // Auslesen der Taster am Analogen Pin A0.
  if (Analogwert > 1000) return KeinTaster;
  if (Analogwert < 50) return Tasterrechts;
  if (Analogwert < 195) return Tasteroben;
  if (Analogwert < 380) return Tasterunten;
  if (Analogwert < 555) return Tasterlinks;
  if (Analogwert < 790) return Tasterselect;

  return KeinTaster; // Ausgabe wenn kein Taster gedrückt wurde.
}
// Hier wird der Programmblock "Tasterstatus" abeschlossen.


//##############################################
//
// Temperatur
//
//##############################################
#include <OneWire.h>
//#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define OFEN_PIN 22
#define SPEICHER_PIN 23
//#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWireBus1(ONE_WIRE_BUS_1);
//OneWire oneWireBus2(ONE_WIRE_BUS_2);


// Pass our oneWire reference to Dallas Temperature.
//DallasTemperature tempSensors1(&oneWireBus1);
//DallasTemperature tempSensors2(&oneWireBus2);

#include <DS1820_Simple.h> 
DS1820_Simple * ofenSensor; //OneWire BUS 1
DS1820_Simple * speicherSensor; //OneWire BUS 1


//28 0E 92 92 04 00 00 41
//DeviceAddress ofenSensorAdresse = { 0x28, 0x0E, 0x02, 0xCD, 0x03, 0x00, 0x00, 0xCE };
byte ofenSensorAdresse[8] = { 0x28, 0x0E, 0x02, 0xCD, 0x03, 0x00, 0x00, 0xCE };
//28 BD CB 92 04 00 00 79
//DeviceAddress speicherSensorAdresse  = { 0x10, 0x9D, 0xDA, 0xDA, 0x00, 0x08, 0x00, 0x8F };
byte speicherSensorAdresse[8] = { 0x10, 0x9D, 0xDA, 0xDA, 0x00, 0x08, 0x00, 0x8F };



//##############################################
//
// Heartbeat
//
//##############################################
#include <Heartbeat.h>
Heartbeat * h;

//##############################################
//
// Relay
//
//##############################################
#include <SwitchRelay.h>
SwitchRelay * PumpeRelay;

void setup()
{
  Serial.begin(9600);
  
  lcd.begin(16, 2); // Starten der LCD Programmbibliothek.
  lcd.clear(); // Clear und Position auf 0,0
  lcd.print(">Init Temp. Sens.");
  delay(1000);


  ofenSensor = new DS1820_Simple;
  speicherSensor = new DS1820_Simple;
  
  ofenSensor->init(OFEN_PIN);
  speicherSensor->init(SPEICHER_PIN);

  ofenSensor->addSensor("Ofen",0,ofenSensorAdresse);
  speicherSensor->addSensor("Speicher",0,speicherSensorAdresse);

  ofenSensor->updateTemp();
  speicherSensor->updateTemp();

  //tempSensors1.begin(); // Starten der Temperatur Sensoren
  //tempSensors2.begin(); // Starten der Temperatur Sensoren

  /*
    lcd.clear();
    lcd.setCursor(0, 0); // Angabe des Cursorstartpunktes oben links.
    lcd.print("Parasite Power:");
    lcd.setCursor(0, 1);

    if (tempSensors1.isParasitePowerMode()) {
      lcd.print("On");
    } else {
      lcd.print("Off");
    }

    delay(5000);
  */
  /*
  lcd.clear();
  lcd.setCursor(0, 0); // Angabe des Cursorstartpunktes oben links.
  lcd.print("Anzahl DS18B20:");
  lcd.setCursor(0, 1);
  lcd.print(tempSensors1.getDS18Count());
*/
  //tempSensors1.setWaitForConversion(false);
  //tempSensors1.setResolution(ofenSensorAdresse, TEMPERATURE_PRECISION);
  //tempSensors1.setResolution(speicherSensorAdresse, TEMPERATURE_PRECISION);
  //tempSensors1.requestTemperatures();

  //tempSensors2.setWaitForConversion(false);
  //tempSensors2.setResolution(ofenSensorAdresse, TEMPERATURE_PRECISION);
  //tempSensors2.setResolution(speicherSensorAdresse, TEMPERATURE_PRECISION);
  //tempSensors2.requestTemperatures();

  delay(5000);

  lcd.setCursor(0, 0); // Angabe des Cursorstartpunktes oben links.
  lcd.print(">EEPROM lesen");

  EEPROM.get(eeAdresse, ep); // EEPROM lesen ab Startadresse in Variable ep

  //Einfache Werteprüfung der gelesenen EEPROM Werte. Falls fehlerhaft überschreiben mit default
  Taster = Tasterstatus(); //Reset wenn SET Taste bei start gedrückt ist bei Start
  if (ep.einschaltDifferenz < 0 || ep.einschaltDifferenz >= 100 || ep.hysterese < 0 || ep.hysterese >= 100 || Taster == Tasterselect) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EEPROM!");
    lcd.setCursor(0, 1);
    lcd.print(">Setzte Default");
    delay(5000);

    ep.einschaltDifferenz = 5;
    ep.hysterese = 2.5;
    EEPROM.put(eeAdresse, ep);
  }
  lcd.clear();
  lcd.print("EEPROM OK :)");
  delay(2000);

  printEinDifferenz();
  delay(5000);

  printHysterese();
  delay(5000);

  PumpeRelay = new SwitchRelay("Pumpe", 52, LOW, 0);

  h = new Heartbeat(13, 1000);
}

// Funktion für Ausgabe der Einschaltdifferenz
void printEinDifferenz() {
  lcd.clear();
  lcd.print("Ein Differenz:");
  lcd.setCursor(0, 1);
  lcd.print(ep.einschaltDifferenz);
}

// Funktion für Ausgabe der Hysterese
void printHysterese() {
  lcd.clear();
  lcd.print("Hysterese:");
  lcd.setCursor(0, 1);
  lcd.print(ep.hysterese);
}

long currentMillis = 0; // Aktuelle Zeit
long previousMillisDisplay = 0; // Display letzte Aktualisierung
long displayUpdateInterval = 1000; // Display Update Interval
long previousMillisTempRequest = 0; // Sensor request Zeitpunkt
long waitForConversion = 1000; // Zeit bis zum abholen der Werte

float speicherTemp = 0; // Temperatur Speicher
float ofenTemp = 0; // Temperatur Ofen

bool pumpeAn = false; // Pumpe einschalten

void loop()
{
  h->blink();

  ofenSensor->updateTemp();
  speicherSensor->updateTemp();

  currentMillis = millis(); // aktuelle Zeit speichern

/*
  if (currentMillis - previousMillisTempRequest > waitForConversion) { //prüfen ob Interval erreicht
    previousMillisTempRequest = currentMillis; // Zeitstempel speichern

    //if (tempSensors1.isConnected(ofenSensorAdresse) && tempSensors1.isConnected(speicherSensorAdresse)) {
    ofenTemp = tempSensors1.getTempC(ofenSensorAdresse);
    speicherTemp = tempSensors2.getTempC(speicherSensorAdresse);
    //}
    tempSensors1.requestTemperatures();
    tempSensors2.requestTemperatures();
  }
  */
  speicherTemp = speicherSensor->getTempByAddress(speicherSensorAdresse);
  ofenTemp = ofenSensor->getTempByAddress(ofenSensorAdresse);

  if (ofenTemp != -127 && speicherTemp != -127) { // prüfen ob Sensor als disc. markiert ist.
    if  (ofenTemp > (speicherTemp + ep.einschaltDifferenz) )
    {
      pumpeAn = true;
      PumpeRelay->set(1);
    }
    else
    {
      if (ofenTemp < (speicherTemp - ep.hysterese) )
      {
        pumpeAn = false;
        PumpeRelay->set(0);
      }
      else
      {
        // "in" der hysterese
      }
    }
  }


  Taster = Tasterstatus(); //Hier springt der Loop in den oben angegebenen Programmabschnitt "Tasterstatus" und liest dort den gedrückten Taster aus.

  switch (Taster) // Abhängig von der gedrückten Taste wird in dem folgenden switch-case Befehl entschieden, was auf dem LCD angezeigt wird.
  {
    
    case Tasterrechts: // Wenn die rechte Taste gedrückt wurde...
      {
        
        if (ep.einschaltDifferenz > 100) {
          delay(1000);
          break;
        }

        ep.einschaltDifferenz = ep.einschaltDifferenz + 0.5;

        EEPROM.put(eeAdresse, ep);

        printEinDifferenz();

        delay(250);
        previousMillisDisplay = currentMillis; // Zeitstempel speichern
       
        break;
      }
    case Tasterlinks:  // Wenn die linke Taste gedrückt wurde...
      {
        
        if (ep.einschaltDifferenz <= 1) {
          delay(1000);
          break;
        }

        ep.einschaltDifferenz = ep.einschaltDifferenz - 0.5;

        EEPROM.put(eeAdresse, ep);

        printEinDifferenz();

        delay(250);
        previousMillisDisplay = currentMillis; // Zeitstempel speichern
       
        break;
      }
      
    case Tasteroben:
      {
        if (ep.hysterese > 100) {
          delay(1000);
          break;
        }

        ep.hysterese = ep.hysterese + 0.25;

        EEPROM.put(eeAdresse, ep);

        printHysterese();

        delay(250);
        previousMillisDisplay = currentMillis; // Zeitstempel speichern
        break;
      }
    case Tasterunten:
      {
        if (ep.hysterese <= 1) {
          delay(1000);
          break;
        }

        ep.hysterese = ep.hysterese - 0.25;

        EEPROM.put(eeAdresse, ep);

        printHysterese();

        delay(250);
        previousMillisDisplay = currentMillis; // Zeitstempel speichern
        break;
      }
    case Tasterselect:
      {
        lcd.clear(); // Clear und Position auf 0,0
        lcd.print("On/Off: ");
        lcd.print(PumpeRelay->getOnOffCount());
        lcd.setCursor(0, 1);
        lcd.print("Flap Count: ");
        lcd.print(PumpeRelay->getFlapCount());

        delay (3000);
        break;
      }
    case KeinTaster:
      {
        if (currentMillis - previousMillisDisplay > displayUpdateInterval) { //prüfen ob Interval erreich
          previousMillisDisplay = currentMillis; // Zeitstempel speichern
          lcd.clear(); // Clear und Position auf 0,0
          lcd.print("O:");
          lcd.print(ofenTemp);
          lcd.print(" S:");
          lcd.print(speicherTemp);
          lcd.setCursor(0, 1);
          if (ofenTemp != -127 && speicherTemp != -127) {
            lcd.print("Di:");
            lcd.print(ofenTemp - speicherTemp);
            if (pumpeAn == true) {
              lcd.print(" P:an");
            } else {
              lcd.print(" P:aus");
            }
          } else {
            lcd.print("Sensor getrennt");
          }

        }
        break;
      }
  } //switch-case Befehl beenden

} //Loop beenden
