#include <Adafruit_MAX31856.h>
#include "LiquidCrystal.h"
#include "Wire.h"

float grilltempcj, grilltemp, foodtempcj, foodtemp; // temp reading variables
float grillset = 150; // grill temp setpoint variable startpoint
float foodset = 150; // food temp setpoint variable startpoint
int x; // analog button variable
int y = 0; // button variable
int a = 0; // counter variable for menue

LiquidCrystal lcd( 8,  9,  4,  5,  6,  7); // sets up display's digital pins

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 foodtempmax = Adafruit_MAX31856(35, 33, 31, 29);
Adafruit_MAX31856 grilltempmax = Adafruit_MAX31856(51, 49, 47, 45);

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  foodtempmax.begin();

  grilltempmax.begin();

  foodtempmax.setThermocoupleType(MAX31856_TCTYPE_K);

  grilltempmax.setThermocoupleType(MAX31856_TCTYPE_K);

  lcd.begin(16, 2);

}

void readtemp() {
  grilltempcj = grilltempmax.readCJTemperature();
  grilltemp = grilltempmax.readThermocoupleTemperature();
  foodtempcj = foodtempmax.readCJTemperature();
  foodtemp = foodtempmax.readThermocoupleTemperature();
}

void button_read() {

  x = analogRead(0);

  if (x < 60) {
   y = 1; //right
  }
  else if (x < 300) {
    y = 2; //up
  }
  else if (x < 500){
    y = 3; //down
   }
  else if (x < 700){
    y = 4; //left
   }
  else if (x < 900){
   y = 5; //select
 }
}

void case_counter() {
  button_read();
    if (y == 1) {
      a++;
      delay(200);
      if (a == 4) {
        a = 0;
      }
    }
    if (y == 4) {
      a--;
      delay(200);
      if (a == -1) {
        a = 3;
      }
    }
    y = 0;
}

void loop() {

  readtemp();
  button_read();
  case_counter();
  
  if (a == 0) {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Food Temp");
         lcd.setCursor(0,1);
         lcd.print(foodtemp);
         y = 0;
     }
  else if (a == 1) {
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Food Setpoint");
       lcd.setCursor(0,1);
       lcd.print(foodset);
       button_read();
       if (y == 2) { //if up pressed, increase foodset
          foodset++;
          y = 0;
          delay(75);
       }
       else if (y == 3) { //if down is pressed, decrease foodset
          foodset--;
          y = 0;
          delay(75);
       }
   }
   else if (a == 2) {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Grill Temp");
         lcd.setCursor(0,1);
         lcd.print(grilltemp);
    }
    else if (a == 3) {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Grill Setpoint");
         lcd.setCursor(0,1);
         lcd.print(grillset);
         button_read();
         if (y == 2) { // if up pressed, increase grillset
           grillset++;
           y = 0;
           delay(75);
         }
         else if (y == 3) { //if up pressed, decrease grillset
           grillset--;
           y = 0;
           delay(75);
         }
    }
  

 
  //Serial.print("Grill Temp Setpoint");
  //Serial.println(grillset);

  //Serial.print("Food Temp Setpoint");
  //Serial.println(foodset);
  
  //Serial.print("Grill Cold Junction Temp: ");
  //Serial.println(grilltempcj);

  //Serial.print("Grill Temp: ");
  //Serial.println(grilltemp);

  //Serial.print("Food Cold Junction Temp: ");
  //Serial.println(foodtempcj);

  //Serial.print("Food Temp: ");
  //Serial.println(foodtemp);
  
}
