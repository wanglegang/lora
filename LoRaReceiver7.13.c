/*
Copyright（c）2017，wanglegang
摘要：LORA无线传感器接收程序
版本：0.1
日期：2017年7月9日
芯片：ATMEGA328
原理：3.3V spi lora 芯片， cpu运行在3.3V下，13487可以运行在3.3V
修改说明：2017年7月9日 启动4号指示灯闪亮3次
*/

#include <avr/wdt.h>   //看门狗
#include <SPI.h>
#include <LoRa.h>
int led=4;
int ReadStr;
unsigned long diandong_time;//落杆时间
void setup() 
{
  pinMode(led,OUTPUT);
    digitalWrite(led,LOW);
    delay(50);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else
  {
    Serial.println("LoRa OK!");
  }
  wdt_enable(WDTO_2S);   //领养看门狗
}
void loop() 
{ wdt_reset();   //喂狗
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet :");
    // read packet
    while (LoRa.available()) 
    {
      ReadStr=LoRa.read();
      Serial.println(ReadStr);  
      if(ReadStr==1)
      {
        digitalWrite(led,HIGH);
        diandong_time=millis(); //继电器点动时间
      }
     /*
      else if (ReadStr==16)
      {
        digitalWrite(led,LOW);
      }
      */
    }   
  }
      if((millis()-diandong_time>50)&&(diandong_time !=0))  //||(millis()-diandong_time>500)&&(diandong_time !=0)) //当前时间和落杆时间的差0.5秒后道闸落杆按钮解锁
      { diandong_time=0; 
        digitalWrite(led,LOW);

      }
}
