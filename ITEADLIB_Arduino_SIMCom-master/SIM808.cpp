#include "SIM808.h"
#include "Streaming.h"
#include "HWSerial.h"

#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 10

//#define RESETPIN 12

//SIMCOM808 gsm;
SIMCOM808::SIMCOM808() {};
SIMCOM808::~SIMCOM808() {};

/**********************************************************
Function: 	This function premits to wake up the module
			(only for SIM908) when it goes in energy saving
			mode.

Author:		Marco Martines
Created:	unknown
Modified:	09/04/2016

Output:		none

Comments:	It would be nice to call this function
 			automatically when gsm.begin is called (of course
 			only if a SIM808 is used). 
**********************************************************/

char SIMCOM808::forceON()
{
	 char ret_val=0;
     char *p_char;
     char *p_char1;

     SimpleWriteln(F("AT+CREG?"));
     WaitResp(5000, 100, str_ok);
     if(IsStringReceived(str_ok)) {
          ret_val=1;
     }
     
     p_char = strchr((char *)(gsm.comm_buf),',');
     p_char1 = p_char+1;
     *(p_char1+2)=0;
     p_char = strchr((char *)(p_char1), ',');
     if (p_char != NULL) {
          *p_char = 0;
     }

     if((*p_char1)=='4') {
          digitalWrite(GSM_ON, HIGH);
          delay(1200);
          digitalWrite(GSM_ON, LOW);
          delay(10000);
          ret_val=2;
     }

     return ret_val;
}

int SIMCOM808::configandwait(char* pin)
{
     int connCode;

     if(pin) setPIN(pin); //syv

     // Try 10 times to register in the network. Note this can take some time!
     for(int i=0; i<10; i++) {
          //Ask for register status to GPRS network.
          SimpleWriteln(F("AT+CGREG?"));

          //Se espera la unsolicited response de registered to network.
          while(gsm.WaitResp(5000, 50, "+CGREG: 0,")!=RX_FINISHED_STR_RECV)
          {
               connCode=_cell.read();
               if((connCode==1)||(connCode==5)) {
                    setStatus(READY);

                    SimpleWriteln(F("AT+CMGF=1")); //SMS text mode.
                    delay(200);
                    // Buah, we should take this to readCall()
                    SimpleWriteln(F("AT+CLIP=1")); //SMS text mode.
                    delay(200);
                    return 1;
               }
          }
     }
     return 0;
}

/**
 * SIMCOM808::read(char* buffer, int buffersize)
 *
 * Waits for data to be readable from the gsm module, reads data until
 * no more is available or the buffer has been filled
 *
 * returns number of bytes read
 *
 */
int SIMCOM808::read(char* result, int resultlength)
{
     char temp;
     int i=0;

#ifdef DEBUG_ON
     Serial.print(F("Starting read..\nWaiting for Data.."));
#endif
     // Wait until we start receiving data
     while(gsm.available()<1) {
          delay(100);
#ifdef DEBUG_ON
          Serial.print(F("."));
#endif
     }

     while(gsm.available()>0 && i<(resultlength-1)) {
          temp=_cell.read();
          if(temp>0) {
#ifdef DEBUG_ON
               Serial.print(temp);
#endif
               result[i]=temp;
               i++;
          }
          delay(1);
     }

     // Terminate the string
     result[resultlength-1]='\0';

#ifdef DEBUG_ON
     Serial.println(F("\nDone.."));
#endif
     return i;
}

int SIMCOM808::readCellData(int &mcc, int &mnc, long &lac, long &cellid)
{
     if (getStatus()==IDLE)
          return 0;
     //_cell.flush();
     SimpleWriteln(F("AT+QENG=1,0"));
     SimpleWriteln(F("AT+QENG?"));
     if(gsm.WaitResp(5000, 50, "+QENG")!=RX_FINISHED_STR_NOT_RECV)
          return 0;

     mcc=_cell.read();
     mcc=_cell.read();
     mnc=_cell.read();
     lac=_cell.read();
     cellid=_cell.read();

     gsm.WaitResp(5000, 50, "+OK");
     SimpleWriteln(F("AT+QENG=1,0"));
     gsm.WaitResp(5000, 50, "+OK");
     return 1;
}

boolean SIMCOM808::readSMS(char* msg, int msglength, char* number, int nlength)
{
     Serial.println(F("This method is deprecated! Please use GetSMS in the SMS class."));
     long index;
     char *p_char;
     char *p_char1;

     //_cell.flush();
     //WaitResp(500, 500);
	 if(AT_RESP_OK != gsm.SendATCmdWaitResp("AT+CMGF=1", 500, 100, "OK", 5))
          return false;
     SimpleWriteln(F("AT+CMGL=\"REC UNREAD\",1"));
     WaitResp(5000, 500);
     if(gsm.IsStringReceived("+CMGL")) {

          //index
          p_char = strchr((char *)(gsm.comm_buf),':');
          p_char1 = p_char+2;//we are on the first char of string
          p_char = p_char1+1;
          *p_char = 0;
          index=atoi(p_char1);

          p_char1 = p_char+1;
          p_char = strstr((char *)(p_char1), "\",\"");
          p_char1 = p_char+3;
          p_char = strstr((char *)(p_char1), "\",\"");
          if (p_char != NULL) {
               *p_char = 0;
          }
          strcpy(number, (char *)(p_char1));
          //////

          p_char1 = p_char+3;
          p_char = strstr((char *)(p_char1), "\",\"");
          p_char1 = p_char+3;

          p_char = strstr((char *)(p_char1), "\n");
          p_char1 = p_char+1;
          p_char = strstr((char *)(p_char1), "\n");
          if (p_char != NULL) {
               *p_char = 0;
          }
          strcpy(msg, (char *)(p_char1));

          SimpleWrite(F("AT+CMGD="));
          SimpleWriteln(index);
          gsm.WaitResp(5000, 50, str_ok);
          return true;
     };
     return false;
};

boolean SIMCOM808::readCall(char* number, int nlength)
{
     int index;

     if (getStatus()==IDLE)
          return false;

     if(gsm.WaitResp(5000, 50, "+CLIP: \"")!=RX_FINISHED_STR_RECV)
     {
          _cell.getString("", "\"", number, nlength);
          SimpleWriteln(F("ATH"));
          delay(1000);
          //_cell.flush();
          return true;
     };
     return false;
};

boolean SIMCOM808::call(char* number, unsigned int milliseconds)
{
     if (getStatus()==IDLE)
          return false;

     SimpleWrite(F("ATD"));
     SimpleWrite(number);
     SimpleWriteln(F(";"));
     delay(milliseconds);
     SimpleWriteln(F("ATH"));

     return true;

}

int SIMCOM808::setPIN(char *pin)
{
     //Status = READY or ATTACHED.
     if((getStatus() != IDLE))
          return 2;

     //_cell.flush();

     //AT command to set PIN.
     SimpleWrite(F("AT+CPIN="));
     SimpleWriteln(pin);

     //Expect str_ok.

     if(gsm.WaitResp(5000, 50, str_ok)!=RX_FINISHED_STR_NOT_RECV)
          return 0;
     else
          return 1;
}

int SIMCOM808::changeNSIPmode(char mode)
{
     //_cell.flush();

     SimpleWrite(F("AT+QIDNSIP="));
     SimpleWriteln(mode);
     if(gsm.WaitResp(5000, 50, str_ok)!=RX_FINISHED_STR_NOT_RECV) return 0;
     //if(!_tf.find(str_ok)) return 0;

     return 1;
}

int SIMCOM808::getCCI(char *cci)
{
     //Status must be READY
     if((getStatus() != READY))
          return 2;

     //_cell.flush();

     //AT command to get CCID.
     SimpleWriteln(F("AT+CCID"));

     //Read response from modem
     _cell.getString("\r\n","\r\n",cci, 21);

     //Expect str_ok.
     if(gsm.WaitResp(5000, 50, str_ok)!=RX_FINISHED_STR_NOT_RECV)
          return 0;
     else
          return 1;
}

int SIMCOM808::getIMEI(char *imei)
{
     //_cell.flush();

     //AT command to get IMEI.
     SimpleWriteln(F("AT+GSN"));

     //Read response from modem

     _cell.getString("\r\n","\r\n",imei, 16);

     //Expect str_ok.
     if(gsm.WaitResp(5000, 50, str_ok)!=RX_FINISHED_STR_NOT_RECV)
          return 0;
     else
          return 1;
}



int SIMCOM808::available()
{
     return _cell.available();
}

uint8_t SIMCOM808::read()
{
     return _cell.read();
}

void SIMCOM808::SimpleRead()
{
     char datain;
     if(_cell.available()>0) {
          datain=_cell.read();
          if(datain>0) {
               Serial.print(datain);
          }
     }
}

void SIMCOM808::SimpleWrite(char *comm)
{
     _cell.print(comm);
}

void SIMCOM808::SimpleWrite(const char *comm)
{
     _cell.print(comm);
}

void SIMCOM808::SimpleWrite(int comm)
{
     _cell.print(comm);
}

void SIMCOM808::SimpleWrite(const __FlashStringHelper *pgmstr)
{
     _cell.print(pgmstr);
}

void SIMCOM808::SimpleWriteln(char *comm)
{
     _cell.println(comm);
}

void SIMCOM808::SimpleWriteln(const __FlashStringHelper *pgmstr)
{
     _cell.println(pgmstr);
}

void SIMCOM808::SimpleWriteln(char const *comm)
{
     _cell.println(comm);
}

void SIMCOM808::SimpleWriteln(int comm)
{
     _cell.println(comm);
}

void SIMCOM808::WhileSimpleRead()
{
     char datain;
     while(_cell.available()>0) {
          datain=_cell.read();
          if(datain>0) {
               Serial.print(datain);
          }
     }
}


