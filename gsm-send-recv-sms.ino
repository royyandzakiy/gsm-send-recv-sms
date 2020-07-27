#include <SoftwareSerial.h>
#include "credentials.h"

//SIM800 TX is connected to Arduino Nano D3/Arduino Uno GPIO3
#define SIM800_TX_PIN 3

//SIM800 RX is connected to Arduino Nano D2/Arduino Uno GPIO2
#define SIM800_RX_PIN 2

//Create software serial object to communicate with SIM800L
SoftwareSerial simSerial(SIM800_TX_PIN, SIM800_RX_PIN); //SIM800L Tx & Rx is connected to Arduino #3 & #2

char msg[150] = "Mantap broq 3";

void sendSMS();
void initialize();
void updateSerial();

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  simSerial.begin(9600);

  initialize();  
  sendSMS();
}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    simSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(simSerial.available()) 
  {
    Serial.write(simSerial.read());//Forward what Software Serial received to Serial Port
  }
}

void initialize() {  
  Serial.println("Initializing...");
  delay(1000);
  
  simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  simSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  simSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  simSerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  
  Serial.println(); 
  Serial.println("Initializing Done."); 
}

void sendSMS() {
  Serial.println("Sending SMS...");
  
  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  simSerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  simSerial.print("AT+CMGS=\"");
  simSerial.print(no_hp_tujuan);
  simSerial.println("\"");
  updateSerial();
  simSerial.print(msg); //text content
  updateSerial();
  simSerial.write(26);

  Serial.println();
  Serial.println("Sending SMS Done.");
}
