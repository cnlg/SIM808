//Simple sketch to send and receive SMS.
#include "DFSIM808.h"
GSM gsm;
GSM_SMS sms;

boolean started=false;
char smsbuffer[160];
char n[20];

void setup()
{
     //Serial connection.
     Serial1.begin(9600);
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
     gsm._cell.setSerial(Serial1);  //模块和开发板通过串口Serial1通信    
    
     if (gsm.begin()) {
          Serial.println("\nstatus=READY");
          started=true;
     } 
     else
        Serial.println("\nstatus=IDLE");

     if(started) {
          //Enable this two lines if you want to send an SMS.
          if (sms.SendSMS("18750762139", "Arduino SMS"))
          Serial.println("\nSMS sent OK");
     }
    

};

void loop()
{
     //读取短信
     if(started) {
          //Read if there are messages on SIM card and print them.
          if(gsm.readSMS(smsbuffer, 160, n, 20)) {
               Serial.println(n);
               Serial.println(smsbuffer);
          }
          delay(1000);
     }
};
