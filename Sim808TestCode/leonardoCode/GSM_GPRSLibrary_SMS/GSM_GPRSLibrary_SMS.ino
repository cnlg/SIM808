#include "SIM808.h"
#include <SoftwareSerial.h>
//#include "HWSerial.h"
//If not used, is better to exclude the HTTP library,
//for RAM saving.
//If your sketch reboots itself proprably you have finished,
//your memory available.
//#include "inetGSM.h"

//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
SMSGSM sms;
//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.


//Simple sketch to send and receive SMS.

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

void setup()
{
     //Serial connection.
     Serial1.begin(9600);
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
      gsm._cell.setSerial(Serial1);
     
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(9600)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

     if(started) {
          //Enable this two lines if you want to send an SMS.
          if (sms.SendSMS("18750762139", "Arduino SMS"))
          Serial.println("\nSMS sent OK");
     }
    

};

void loop()
{
     if(started) {
          //Read if there are messages on SIM card and print them.
          if(gsm.readSMS(smsbuffer, 160, n, 20)) {
               Serial.println(n);
               Serial.println(smsbuffer);
          }
          delay(1000);
     }
};
