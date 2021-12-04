#include <Adafruit_MAX31856.h>
#include "LiquidCrystal.h"
#include "Wire.h"
#include <Adafruit_MotorShield.h>

float grilltempcjC, grilltempC, foodtempcjC, foodtempC; // celcius temp reading variables
float grilltempcj, grilltemp, foodtempcj, foodtemp; // fahrenheit temp reading variables
float grillset = 200; // grill temp setpoint variable startpoint
float foodset = 150; // food temp setpoint variable startpoint

int x; // analog button variable
int y = 0; // button variable
int a = 0; // counter variable for menu
int b = 0; // fan case counter variable

LiquidCrystal lcd( 8,  9,  4,  5,  6,  7); // sets up display's digital pins

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 foodtempmax = Adafruit_MAX31856(35, 33, 31, 29); // sets pins for food temp
Adafruit_MAX31856 grilltempmax = Adafruit_MAX31856(51, 49, 47, 45); // sets pins for grill tmep

void setup() {
  Serial.begin(9600);
  //while (!Serial) delay(10);

  foodtempmax.begin();

  grilltempmax.begin();

  foodtempmax.setThermocoupleType(MAX31856_TCTYPE_K);

  grilltempmax.setThermocoupleType(MAX31856_TCTYPE_K);

  lcd.begin(16, 2);

  AFMS.begin();
  myMotor->setSpeed(150); //idk bout these three lines but they in the DC motor test and it works
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
  
}

void readtemp() { //reading temperatures
  grilltempcjC = grilltempmax.readCJTemperature();
  grilltempC = grilltempmax.readThermocoupleTemperature();
  foodtempcjC = foodtempmax.readCJTemperature();
  foodtempC = foodtempmax.readThermocoupleTemperature();

  grilltempcj = ((1.8 * (grilltempcjC)) + 32);
  grilltemp = ((1.8 * (grilltempC)) + 32);
  foodtempcj = ((1.8 * (foodtempcjC)) + 32);
  foodtemp = ((1.8 * (foodtempC)) + 32);
}

void button_read() { // reading the analog pin 0 to determine which lcd keypad button has been pressed

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

void lcd_case_counter() { // counter to scroll through lcd screens bc it seemed easiest
  button_read();
    if (y == 1) {
      a++;
      delay(200);
      if (a == 3) {
        a = 0;
      }
    }
    if (y == 4) {
      a--;
      delay(200);
      if (a == -1) {
        a = 2;
      }
    }
    y = 0;
}

void fan_case_counter() { // sets b to different values based on conditions
  if ((grilltemp < (grillset*0.5)) && (foodtemp < foodset)) {
    b = 1;
  }
  else if ((grilltemp < (grillset*0.75)) && (foodtemp < foodset)) {
    b = 2;
  }
  else if ((grilltemp < (grillset*0.85)) && (foodtemp < foodset)) {
    b = 3;
  }
  else if ((grilltemp < (grillset*0.95)) && (foodtemp < foodset)) {
    b = 4;
  }
  else if ((grilltemp < (grillset*0.98)) && (foodtemp < foodset)) {
    b = 5;
  }
  else if ((grilltemp > grillset) || (foodtemp > foodset) || (grilltemp == grillset) || (foodtemp == foodset)) {
    b = 6;
  }
  Serial.print("b = ");
  Serial.print(b);
  Serial.print(" ");
}

void fan_on1() { // turns fan on for 15seconds . this works in dc motor test, but not in here
  uint8_t i;

  Serial.print("fan on 1");

  myMotor->run(FORWARD);
  for (i=254; i<255; i++) {
    myMotor->setSpeed(i);
    delay(15000);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

}

void fan_on2() { // turns fan on for 7.5 seconds . this works in dc motor test, but not in here
  uint8_t i;

  Serial.print("fan on 2");

  myMotor->run(FORWARD);
  for (i=254; i<255; i++) {
    myMotor->setSpeed(i);
    delay(7500);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

}

void fan_on3() { // turns fan on for 5seconds . this works in dc motor test, but not in here
  uint8_t i;

  Serial.print("fan on 3");

  myMotor->run(FORWARD);
  for (i=254; i<255; i++) {
    myMotor->setSpeed(i);
    delay(5000);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

}

void fan_on4() { // turns fan on for 2.5 seconds . this works in dc motor test, but not in here
  uint8_t i;

  Serial.print("fan on 4");

  myMotor->run(FORWARD);
  for (i=254; i<255; i++) {
    myMotor->setSpeed(i);
    delay(2500);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

}

void fan_on5() { // turns fan on for 1 second . this works in dc motor test, but not in here
  uint8_t i;

  Serial.print("fan on 5");

  myMotor->run(FORWARD);
  for (i=254; i<255; i++) {
    myMotor->setSpeed(i);
    delay(1000);
  }
  for (i=255; i!=0; i--) {
    myMotor->setSpeed(i);
    delay(10);
  }

}



void fan_control() { // litterally its name
  uint8_t i;
  if (b == 1) {
    fan_on1(); 
    Serial.print("Fan case = 1");
    Serial.println(" ");
  }
  if (b == 2) {
    fan_on2();
    Serial.print("Fan case = 2");
    Serial.println(" ");
  }
  if (b == 3) {
    fan_on3();
    Serial.print("Fan case = 3");
    Serial.println(" ");
  }
  if (b == 4) {
    fan_on4();
    Serial.print("Fan case = 4");
    Serial.println(" ");
  }
  if (b == 5) {
    fan_on5();
    Serial.print("Fan case = 5");
    Serial.println(" ");
  }
  if (b == 6) {
    myMotor->run(RELEASE);
    myMotor->setSpeed(0);
    delay(1000);
    Serial.print("Fan case = 6");
    Serial.println(" ");
  }
  readtemp();
  //button_read();
  //lcd_case_counter();
}

void loop() {

  readtemp(); // start out by reading temps
  button_read(); // next read button value
  lcd_case_counter(); // determine if right or left is pressed to change screen
  if (a == 0) { // screen 1 - food setpoint
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
  else if (a == 1) { // screen 2 - grill setpoint
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
   else if (a == 2) { // ask to start controlling the grill basically, can still press left or right to go back to setpoints
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Press Select to");
         lcd.setCursor(0,1);
         lcd.print("Run");
         button_read();
         if (y == 5) { // if up pressed, increase grillset
           while (y != 1) { // litterally just making it loop here, not the issue, im 99%sure bc it keeps updating food and grill temp, so it is looping
            readtemp();
            lcd.setCursor(0,0); 
            lcd.print("Food       Grill");
            lcd.setCursor(0,1);
            lcd.print(foodtemp);
            lcd.setCursor(10,1);
            lcd.print(grilltemp);
            fan_case_counter(); // setting fan case value b
            fan_control(); // supposed to turn on the fan and read the temp again
            delay(20000); //delay 20 seconds before turning on fan control again
           }
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
  //Serial.println(" ");
  
}
