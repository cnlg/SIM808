#include "DFSIM808.h"
GSM gsm;
GSM_GPS gps;

char lon[15];
char lat[15];
char alt[15];
char Time[20];
char vel[15];
char msg1[5];
char msg2[5];

double longitude;
double latitude;
char message[128];

char stat;
char inSerial[20];
int i=0;
boolean started=false;

unsigned long lastTime = 0;

void setup()
{
     //Serial connection.
     Serial.begin(9600);
     gsm._cell.setSerial(Serial);

     if (gsm.begin()) {
          Serial.println("\nstatus=READY");
          gsm.forceON();  //To ensure that SIM908 is not only in charge mode
          started=true;
     } else Serial.println("\nstatus=IDLE");

     if(started) {
          //GPS attach
          if (gps.attachGPS())
               Serial.println("status=GPSREADY");
          else Serial.println("status=ERROR");

          delay(20000); //Time for fixing
          stat=gps.getStat();
          if(stat==1)
               Serial.println("NOT FIXED");
          else if(stat==0)
               Serial.println("GPS OFF");
          else if(stat==2)
               Serial.println("2D FIXED");
          else if(stat==3)
               Serial.println("3D FIXED");
          delay(5000);
          //Get data from GPS
          gps.getPar(lon,lat,alt,Time,vel);
          Serial.println(lon);
          Serial.println(lat);
          Serial.println(alt);
          Serial.println(Time);
          Serial.println(vel);
     }
};

void loop()
{
    //通过串口查看输出的GPS信息
     serialswread();

//     if(millis() - lastTime > 4000)
//     {
//        gps.getPar(lon,lat,alt,Time,vel);
//        longitude = gps.convertLong(lon);//经度转换成10进制
//        latitude = gps.convertLat(lat); //纬度转换成10进制
//        itoa(longitude, message, 10); 
//          Serial.println(longitude);  
//          Serial.println(latitude);   
//     }
}

void serialswread()
{
     gsm.SimpleRead();
}

