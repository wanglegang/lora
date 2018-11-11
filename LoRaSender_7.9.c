/*
Copyright（c）2017，wanglegang
摘要：LORA无线传感器发送程序
版本：0.1
日期：2017年7月9日
芯片：ATMEGA328
原理：3.3V spi lora 芯片， cpu运行在3.3V下，13487可以运行在3.3V
修改说明：2017年7月9日 LoRa.beginPacket();LoRa.endPacket();放在loop根下
*/
#include <avr/wdt.h>   //看门狗
#include <SPI.h>
#include <LoRa.h>
//int counter = 0;
#define lora_HIGH  A3  //传感器高电平触发A3
int lora_HIGH_biaoji=0;//lora 高电平标记
int lora_LOW_biaoji=0; //lora 低电平标记
int io_led_yunxing = 5;//运行指示灯
unsigned long run_high_time;//指示灯亮的时间
unsigned long run_low_time;//指示灯灭的时间
unsigned int run_led_time=999;//指示灯持续亮的时间
unsigned  long  xintiao_jiange_time=10000;//心跳间隔时间
unsigned  long  xintiao_time ;//心跳时间
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {   //成功连接lora芯片不打印失败
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(lora_HIGH,INPUT); //传感器引脚设置输入
  digitalWrite(lora_HIGH, HIGH); //设置上拉电阻  
  wdt_enable(WDTO_2S);   //领养看门狗
}
void loop() {
  wdt_reset();   //喂狗
  //LoRa.beginPacket();//lora准备接收数据
  if(digitalRead(lora_HIGH) == LOW)  //高电平触发传感器引脚
        { 
          if (lora_LOW_biaoji==0) //判断标记位是否等于0
            {
              Serial.println("01 "); //串口输出01，
              LoRa.beginPacket();
              //LoRa.print("LOW ");
              LoRa.write(0x01); //lora 发送16进制01， 对方接收是01
              LoRa.endPacket();
              digitalWrite(io_led_yunxing, HIGH);  //运行灯亮
              lora_LOW_biaoji=1; //标记为1
            }
        } 
    else 
        {     lora_LOW_biaoji=0; //标记为0
        }
   
   if(digitalRead(lora_HIGH) == HIGH) 
        { 
          if (lora_HIGH_biaoji==0)
            { Serial.println("10 ");
             LoRa.beginPacket();
             // LoRa.print("HIGH ");
              LoRa.write(0x10);
             LoRa.endPacket();
              digitalWrite(io_led_yunxing, LOW);  //灯亮
              lora_HIGH_biaoji=1;
            }
        } 
    else 
        {     lora_HIGH_biaoji=0;
        }
       
    
         //=======================================================以下运行灯闪烁，勿动===============================================
 
     if((digitalRead(io_led_yunxing) == LOW)&&(millis()-run_high_time>run_led_time)) //当黄闪灯接口20为低电平时且当前时间比黄灯高电平时的时间差大于0.5秒时，频率是1hz
        {
          digitalWrite(io_led_yunxing, HIGH);  //灯亮，20口输出高电平
          run_low_time=millis(); //记住低电平时的时间
        }
     if((digitalRead(io_led_yunxing) == HIGH)&&(millis()-run_low_time>run_led_time))//当黄闪灯接口20为高电平时且当前时间比黄灯低电平时的时间差大于0.5秒时，频率是1hz
        {
          digitalWrite(io_led_yunxing, LOW);  //灯灭，20口输出低电平
          run_high_time=millis();//记住高电平时的时间
        }
      
  
        //=======================================================心跳时间================================
        if((millis()-xintiao_time>10000)) //当前时间减去心跳时间大于设定的间隔心跳时间的时候执行
        {    //LoRa.endPacket(); //结束发送
             //LoRa.beginPacket();//lora准备接收数据
             Serial.println("a ");
              LoRa.beginPacket();
             // LoRa.print("HIGH ");
              LoRa.write(0xa);
               Serial.println(millis());
               Serial.println( xintiao_time);
              xintiao_time=millis(); //记住发送心跳包的时间
              digitalWrite(io_led_yunxing, LOW);  //灯亮
             LoRa.endPacket(); //结束发送
        }

        // LoRa.endPacket(); //结束发送
  }
