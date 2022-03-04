//pump 
#define PUMP_PIN 4
void Water_Pumb(bool Mode);
 
//rain sensor
#define Rain_pin 3
bool Rain_Sensor();
bool rain=false;
 
//soil sensor
#define soil_pin A0
float moisture_percentage;  
void soil_sensor();
 
// led:
#define ledPin 2
void Led(bool flg);
 
//light sensor:
#define ldr_pin A2
int ldr = 0;
void LDR();
 
//tempreture:
#define lm35_pin A1
int lm35 = 0;
double temper;
float Temperature();
bool in_timer=false;
// LCD:
//    RS, E,  D4, D5, D6, D7
#include <LiquidCrystal.h> 
LiquidCrystal lcd(12, 11, 10,  9,  8,  7);
void Show(String s, int col, int row);
void Show(String s);
 
 
// Time:
#include <string.h>
 
int seconds =0, minutes=30, hours=9;
void Time();
// watering counter
int watering_counter=0;
 
 
 
void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(soil_pin,INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Rain_pin, INPUT);
  lcd.begin(16, 2);
  lcd.home();
  Serial.begin(9600);
 
 
}
 
void loop() {
 
  soil_sensor();
  Time(); 
   LDR();
   temper= Temperature();
 
  delay(1000);
}
 
 
 
void Time()
{
  //cal seconds,minutes, hours
  seconds++;
  if (seconds ==60)
  {
    seconds=0;
    minutes++;
  }
  if (minutes ==60)
  {
    minutes=0;
    hours++;
  }
  if (hours==24)
  {
    minutes=hours=seconds=0;
  }
// converting time to string 
  String sec=String(seconds);
  sec= (seconds<=9)?"0"+sec: sec;
  String minu=String(minutes);
  minu= (minutes<=9)?"0"+minu: minu;
  String h=String(hours);
  h= (hours<=9)?"0"+h: h;
  String total_time=h+":"+minu+":"+sec;
  // displaying time on LCD
  Show(total_time,0,0);
 
 
  //if it rains the next watering will be canceled
 
  if (hours < 9 && !rain )
      {rain= Rain_Sensor();}
      if (hours ==9 && minutes == 15)
    {rain =false;}
   if ((hours>19 && hours<16) && !rain)
    {rain= Rain_Sensor();}
 
    if (hours >16)
    {rain =false;}
 
 
  // Determine the number of times to water the plant in the afternoon at 2PM
  // if temperature is greater than or equal 29 C -> water 3 times, otherwise water only two times
 
  if (hours ==14&& minutes<30 && (temper>= 29))
  {
 
    watering_counter=3;
  }
  else 
  {
    watering_counter=2;
  }
 
  //watering at hour: 9Am and 16(4PM)  if moisture percentage larger than 70% watering will be canceled 
  // watering is canceled as well if it rains.
  if ((hours==9 || hours==16)&&(minutes==0)&&(seconds<10)  &&(moisture_percentage<70)&& !rain)
  {
    in_timer=true;
    Water_Pumb(true);
 
  }
 
            // the third time the plant may be watered in is 9 PM 
 else if ((hours==21)&&( watering_counter==3 )&&(minutes==0) &&(seconds<10) &&(moisture_percentage<50) && !rain)
  {
    in_timer=true;
    Water_Pumb(true);
 
  } 
  else
  {in_timer=false;}
 
}
// LCD function passing a string and diplaying it.
void Show(String s, int col, int row) {
 
  lcd.setCursor(col, row);
  lcd.print(s);
}
void Show(String s) {
  lcd.home();
 
  lcd.print(s);
}
// solid moisture function
void soil_sensor()
{
 int soilvalue = analogRead(soil_pin); 
 moisture_percentage = ( 100 - ( (soilvalue/1023.00) * 100 ) );
 
   //if the soil is too dry the pump will be opened
 if (moisture_percentage<=15.00  &&!in_timer) Water_Pumb(true);
 else if(moisture_percentage>15.00  &&!in_timer)  Water_Pumb(false);
 
 // displaying moisture percentage on the LCD
 if (moisture_percentage<10.0  )
 {
  Show(String(int(moisture_percentage))+" % ",9,1);
  }else
  {
    Show(String(int(moisture_percentage))+"% ",9,1);   
  }
 
 
}
 
 
float Temperature() {
  lm35 = analogRead(lm35_pin);
  //  the calculating of temperature in Celcius scale
 
  float temperature = (lm35 / 1024.0) * 500.0;
 
 
  Show(String(int(temperature))+"C",10,0);
  return temperature;
 
}
void LDR() {
  ldr = analogRead(ldr_pin);
  if(ldr < 60) {
    Led(true);
  }else{
    Led(false);
  }
}
 
void Water_Pumb(bool Mode )
{
  digitalWrite(PUMP_PIN, !Mode);
  if (Mode) Show("Pump ON ",0,1);
  else Show("Pump OFF",0,1);
}
void Led(bool flg)
    {
      if(flg == true )
      {
        digitalWrite(ledPin,HIGH);
      }
      else
      {
        digitalWrite(ledPin,LOW);
      }
    }
 
 
//  rain function
bool Rain_Sensor()
{
  return !digitalRead(Rain_pin);
 
}
 
