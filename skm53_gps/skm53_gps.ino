//How to use SKM53 GPS Shieldwith Arduino or CT-UNO
//More infor available at http://tutorial.cytron.com.my/

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

LiquidCrystal lcd (8,9,4,5,6,7);  //pin initialization for lcd
SoftwareSerial ss(2,3);           //software serial pin for skm53 shield
TinyGPS gps;

const int button_key=A0;          //button pin for lcd keypad shield

//variables for gps
boolean newData=false;
float flat,flon;
int year;byte month,day,hour,minute,second;

boolean state=0;

void setup() 
{
  pinMode(button_key,INPUT);
  lcd.begin(16,2);
  lcd.print("Acquiring GPS...");
  ss.begin(9600);
}

void loop() 
{
  if(analogRead(button_key)>600&&analogRead(button_key)<650)    //read the lcd keypad button
  state^=1;
 
  while(ss.available())
  {
    char c=ss.read();
    if(gps.encode(c))
    {
      newData=true;
      gps.f_get_position(&flat,&flon);
      gps.crack_datetime(&year,&month,&day,&hour,&minute,&second);
      hour=hour+8;           //hour +8 for GMT+8 time zone
    }
  }
  
  if(newData)
  {
    lcd.clear();
    if(state==0)              //display latitude and longitude
    {
      lcd.setCursor(0,0);
      lcd.print("Lat:");
      lcd.setCursor(6,0);
      lcd.print(flat);
      
      lcd.setCursor(0,1);
      lcd.print("Lon:");
      lcd.setCursor(6,1);
      lcd.print(flon);
    }
    else if(state==1)        //display date and time
    {
      lcd.setCursor(0,0);
      lcd.print("Date:");
      lcd.setCursor(6,0);
      lcd.print(day);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(year);
      
      lcd.setCursor(0,1);
      lcd.print("Time:");
      lcd.setCursor(6,1);
      lcd.print(hour);
      lcd.print(":");
      lcd.print(minute);
      lcd.print(":");
      lcd.print(second);
    }
    newData=false;
    delay(10);
  }
}
