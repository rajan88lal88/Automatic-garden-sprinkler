#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
//Connect RTC and OLED SDA pin to Arduino Uno Pin A4
//Connect RTC and OLED SLC pin to Arduino Uno Pin A4
//VCC pin of OLED and RTC to 3.3v
//VCC of Relay module to be connected to 5V.
//GND pin of all the modules to be connected to any GND of arduino
#define R1 3 //Relay 1
#define R2 4 //Relay 2
#define rain A0//rain sensor AO pin to arduino A0 pin
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
int next;
char r1_status[4];
char r2_status[4];
int sensor_val,h,m,s;
void setup()
{
  
  //Serial.begin(9600);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3x
   
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(rain,INPUT);
  
}
void loop()
{
    
    tmElements_t tm;
    /*Serial.print(sensor_val);
    Serial.print(" : ");
    Serial.print(r1_status);
    Serial.print(" : ");
    Serial.print(r2_status);
    Serial.print("\n");
    
    */
    sensor_val = analogRead(rain);
    if(sensor_val>750)// change 750 to desired value of moisture to activate relay 2
    {
       digitalWrite(R2,LOW);
       strcpy(r2_status,"ON");
    }
    else
    {
       digitalWrite(R2,HIGH);
       strcpy(r2_status,"OFF");
    }
    if(RTC.read(tm))
    {
      
    if(tm.Month>=10||tm.Month<6||(tm.Month==6&&tm.Day<=5))
    {
       if(tm.Hour==8||tm.Hour==16)
       {
          digitalWrite(R1,LOW);
          strcpy(r1_status,"ON");
          m=tm.Minute;
          h=0;
          s=tm.Second;
       }
       else
       {
          digitalWrite(R1,HIGH);
          strcpy(r1_status,"OFF");
          s=59;
          m=59;
          h=59;
       }
    }
    if((tm.Month==6&&tm.Day>=6)||(tm.Month>6&&tm.Month<10))
    {
       if((tm.Hour==8&&tm.Minute<1)||(tm.Hour==16&&tm.Minute<1))
       {
          digitalWrite(R1,LOW);
          strcpy(r1_status,"ON");
          s=tm.Second;
          m=59;
          h=0;
       }
       else 
       {
          digitalWrite(R1,HIGH);
          strcpy(r1_status,"OFF");
          s=59;
          m=59;
          h=0;
       }
    }
    if(tm.Hour>8&&tm.Hour<16)
        next=16;
    else
      next=8;
    
    
    }
    
    display.clearDisplay();
    display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10,0);
      display.print(tm.Day);
      display.write('/');
      display.print(tm.Month);
      display.write('/');
      display.print((tmYearToCalendar(tm.Year)%2000));
      display.write(' ');
      display.write(' ');
      print2digits(tm.Hour);
      display.write(':');
      print2digits(tm.Minute);
      display.write(':');
      print2digits(tm.Second);
      display.setCursor(10,8);
      display.print("Next :  ");
      print2digits(next);
      display.print(":00:00");
      display.setCursor(10,17);
      display.print("Left : ");
      print2digits(0);
      display.write(':');
      print2digits(59-m);
      display.write(':');
      print2digits(59-s);
      display.setCursor(10,25);
      display.print("R1 : ");
      display.print(r1_status);
      //display.setCursor(10,15);
      display.print("   R2 : ");
      display.println(r2_status);
      
      display.display();
    delay(100);
}
void print2digits(int number) 
{
  if (number >= 0 && number < 10) 
  {
    display.write('0');
    // display.display();
  }
  display.print(number);
   //display.display();
  //display.println();
}
