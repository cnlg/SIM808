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

boolean started=false;
char str_perc[3];
char str_vol[10];
int net_status;
int gps_status;
unsigned long lastTime = 0;
void setup()
{
     //Serial connection.
     Serial.begin(9600);
     Serial1.begin(9600);
     gsm._cell.setSerial(Serial1);  
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin()) {
          Serial.println("\nstatus=READY");
          started=true;
     } 
     else 
          Serial.println("\nstatus=IDLE");
};

void loop()
{
     if(millis() - lastTime > 2000 ) {
      lastTime = millis();
          if(gsm.getBattInf(str_perc,str_vol))
          {
            Serial.print("str_perc = ");
            Serial.print(str_perc);
            Serial.print("\t");
            Serial.print("str_vol = ");
            Serial.println(str_vol);  
          }
          if(gsm.getNetStatus(net_status))
          {
            Serial.print("net status = ");
            Serial.println(net_status);
          }       
          if(gsm.getGpsStatus(gps_status))
          {
            Serial.print("gps status = ");
            Serial.println(gps_status);
          } 
          Serial.println();   
     }
    
}
