//We have to create the classes for SMSs and calls.
#include "DFSIM808.h"

GSM gsm;
GSMVoiceCall call;
GSM_SMS sms;

char number[20];
byte stat=0;
int value=0;
int pin=1;
char value_str[5];

void setup()
{
    // pinMode(pin,INPUT);
     //Serial connection.
     Serial.begin(9600);
     gsm._cell.setSerial(Serial);
    
     if (gsm.begin()){
          while(!gsm.WritePhoneNumber(1,"18750762139","Jason")){  
          Serial.println("write number failed");
          delay(1000);
          }
            Serial.println("write number sucessed");
           // call.Call("18750762139");
           //Make a phone call
           //call.Call(1); //拨打电话本1位置的号码
           //call.Call("XXXXXXXXXXXXXXX"); //拨打电话号码XXXXXXXXXXXX
     }
     else Serial.println("\nstatus=IDLE");
};

void loop()
{    
     //Chekcs status of call
     //监听是否是指定电话本中的号码来电
     stat=call.CallStatusWithAuth(number,1,3); 
     Serial.println(stat);
     //If the incoming call is from an authorized number
     //saved on SIM in the positions range from 1 to 3.
     //如果是指定号码的电话来电，挂断电话，通过短信发送引脚1的状态
     if(stat==CALL_INCOM_VOICE_AUTH) {   
          //Hang up the call.
          call.HangUp();
         // call.PickUp();
          delay(2000);
          //Check the value of the input.
          value=digitalRead(1);
          //Convert the int to a string.
          itoa(value,value_str,10);
          //Send an SMS to the previous number with
          //the value read previously.
          sms.SendSMS(number,value_str);
     }
     delay(1000);
};
