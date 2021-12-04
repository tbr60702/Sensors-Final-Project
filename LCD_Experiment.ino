#include "LiquidCrystal.h"

LiquidCrystal lcd( 8,  9,  4,  5,  6,  7);
int x;

void setup() {
 lcd.begin(16, 2);
 
 lcd.setCursor(0,0);
 lcd.print("FOOD TEMP");

 Serial.begin(9600);
}

void button_read() {

  x = analogRead(0);

  if (x < 60) {
   x = 1;
  }
  else if (x < 300) {
    x = 2;   
  }
  else if (x < 500){
    x = 3;
   }
  else if (x < 700){
    x = 4;
   }
  else if (x < 900){
   x = 5;
 }
}

void loop() {

 button_read();
 
 lcd.setCursor(10,1);
 
 if (x == 1) {
   lcd.print ("Right ");
 }
 else if (x == 2) {
   lcd.print ("Up    ");
 }
 else if (x == 3) {
   lcd.print ("Down  ");
 }
 else if (x == 4){
   lcd.print ("Left  ");
 }
 else if (x == 5 ){
   lcd.print ("Select");
 }
  Serial.println(x);
 
} 
