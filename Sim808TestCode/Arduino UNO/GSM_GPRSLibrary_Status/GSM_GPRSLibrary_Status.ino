#include "DFSIM808.h"
GSM gsm;

boolean started=false;
int str_perc;  //电池电量百分比
int str_vol;   //电池电压
int net_status; //网络状态：0：未注册 1：注册
int gps_status; //GPS状态： 0：GPS关闭 1：GPS打开
unsigned long lastTime = 0;
void setup()
{
     //Serial connection.
     Serial.begin(9600);
     gsm._cell.setSerial(Serial);  
     if (gsm.begin()) {
          Serial.println("\nstatus=READY");
          started = true;
     } 
     else 
          Serial.println("\nstatus=IDLE");
    if(started){
      //设置模块工作模式
      //1.睡眠模式
      //2.最小功能模式
      //3 全功能模式
      //4 飞行模式
      if(gsm.setModuleMode(3))
        Serial.println("Set the module model success");
    }
};

void loop()
{
   
          if(gsm.getBattInf(str_perc,str_vol))  //检测电池电量
          {
            Serial.print("str_perc = ");
            Serial.print(str_perc);
            Serial.print("\t");
            Serial.print("str_vol = ");
            Serial.println(str_vol);  
          }
          delay(1000);
          if(gsm.getNetStatus(net_status))  //检测网络状态
          {
            Serial.print("net status = ");
            Serial.println(net_status);
          }    
          delay(1000);   
          if(gsm.getGpsStatus(gps_status))  //检测GPS状态
          {
            Serial.print("gps status = ");
            Serial.println(gps_status);
          } 
          Serial.println();   
          delay(1000); 
     
    
}
