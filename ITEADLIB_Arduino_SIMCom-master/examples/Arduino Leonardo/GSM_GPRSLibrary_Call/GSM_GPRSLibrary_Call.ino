

void setup()
{
     pinMode(pin,INPUT);
     Serial1.begin(9600);
     //Serial connection.
     Serial.begin(9600);
     gsm._cell.setSerial(Serial1);
     Serial.println("GSM Shield testing.");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin()){
          //在电话本第1个位置存入电话号码XXXXXXXXXXX，标记为"name"
          //当使用指定号码拨号和接听的时候需要在电话本中是先存好号码
          while(!gsm.WritePhoneNumber(1,"18750762139","Jason")){  
          Serial.println("write number failed");
          }
            Serial.println("write number sucessed");
           //Make a phone call
           //call.Call(1); //拨打电话本1位置的号码
           //call.Call("XXXXXXXXXXXXXXX"); //拨打电话号码XXXXXXXXXXXX
     }
     else
        Serial.println("\nstatus=IDLE");
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
