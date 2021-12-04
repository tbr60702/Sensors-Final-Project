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

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //sets address of motorhat to standard I2C
Adafruit_DCMotor *Fan = AFMS.getMotor(1); //names fan as the M1 connection on the Adafruit motor hat

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 foodtempmax = Adafruit_MAX31856(35, 33, 31, 29); // sets pins for food temp
Adafruit_MAX31856 grilltempmax = Adafruit_MAX31856(51, 49, 47, 45); // sets pins for grill tmep

void setup() {
  Serial.begin(9600);
  //while (!Serial) delay(10);

  foodtempmax.begin(); //initializes the food temp max31856 chip

  grilltempmax.begin(); //initializes the grill temp max31856 chip

  foodtempmax.setThermocoupleType(MAX31856_TCTYPE_K); //sets thermocouple type as K

  grilltempmax.setThermocoupleType(MAX31856_TCTYPE_K); //sets thermocouple type as K

  lcd.begin(16, 2); // initializes the lcd screen as 16 columns and 2 rows

  AFMS.begin(); // initializes the motor hat
  Fan->setSpeed(150); //idk bout these three lines but they in the DC motor test and it works
  Fan->run(FORWARD);
  // turn on motor
  Fan->run(RELEASE);
  
}

void readtemp() { //reading temperatures
  grilltempcjC = grilltempmax.readCJTemperature(); // cold junction and tip readings in celcius
  grilltempC = grilltempmax.readThermocoupleTemperature();
  foodtempcjC = foodtempmax.readCJTemperature();
  foodtempC = foodtempmax.readThermocoupleTemperature();

  grilltempcj = ((1.8 * (grilltempcjC)) + 32); //celcius to Fahrenheit conversion
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
  // goal is for a slow ramp up to the grill set value. Send puffs of air close to grill set to prevent overshoot
  // cut fan off when food is approaching done. let fire begin to choke at 10 degrees short of foodset value to prevent overcooking
  if ((grilltemp < (grillset*0.5)) && (foodtemp < (foodset - 10)) { // grill temp is less than half the grillset value and foodtemp is less than foodset
    b = 1;
  }
  else if ((grilltemp < (grillset*0.75)) && (foodtemp < (foodset - 10))) { // grill temp is less than 75% the grillset value and foodtemp is less than foodset
    b = 2;
  }
  else if ((grilltemp < (grillset*0.85)) && (foodtemp < (foodset -10))) { // grill temp is less than 85% the grillset value and foodtemp is less than foodset
    b = 3;
  }
  else if ((grilltemp < (grillset*0.95)) && (foodtemp < (foodset - 10))) { // grill temp is less than 95% the grillset value and foodtemp is less than foodset. 
    b = 4;
  }
  else if ((grilltemp < (grillset*0.98)) && (foodtemp < (foodset - 10))) { // grill temp is less than 98% the grillset value and foodtemp is less than foodset
    b = 5;
  }
  else if ((grilltemp > grillset) || (grilltemp == grillset) || (foodtemp > (foodset - 10)) || (foodtemp == (foodset - 10))) {
    // cut fan off to choke fire if food is within 10 degrees of done or grill reaches desired temperature
    b = 6;
  }
  //Serial.print("b = "); // uncomment to see b value for debugging
  //Serial.print(b);
  //Serial.print(" ");
}

void fan_on1() { // turns fan on for 15seconds . this works in dc motor test, but not in here
  uint8_t i;

  //Serial.print("fan on 1"); // uncomment for debugging to makesure function is getting called

  Fan->run(FORWARD); // turns fan on
  for (i=254; i<255; i++) { //sets speed to max for 15 secons
    Fan->setSpeed(i);
    delay(15000);
  }
  for (i=255; i!=0; i--) { //slows fan to stop
    Fan->setSpeed(i);
    delay(10);
  }

}

void fan_on2() { // turns fan on for 7.5 seconds . this works in dc motor test, but not in here
  uint8_t i;

  //Serial.print("fan on 2"); // uncomment for debugging to makesure function is getting called

  Fan->run(FORWARD); //turns fan on
  for (i=254; i<255; i++) { //sets speed to max for 7.5 seconds
    Fan->setSpeed(i);
    delay(7500);
  }
  for (i=255; i!=0; i--) { //slows fan to stop
    Fan->setSpeed(i);
    delay(10);
  }

}

void fan_on3() { // turns fan on for 5seconds . this works in dc motor test, but not in here
  uint8_t i;

  // Serial.print("fan on 3"); //uncomment for debugging to makesure function is getting called

  Fan->run(FORWARD); //turns fan on
  for (i=254; i<255; i++) { //sets speed to max for 5 seconds
    Fan->setSpeed(i);
    delay(5000);
  }
  for (i=255; i!=0; i--) { //slows fan to stop
    Fan->setSpeed(i);
    delay(10);
  }

}

void fan_on4() { // turns fan on for 2.5 seconds . this works in dc motor test, but not in here
  uint8_t i;

  // Serial.print("fan on 4"); //uncomment for debugging to makesure function is getting called

  Fan->run(FORWARD); //turns fan on
  for (i=254; i<255; i++) { //sets speed to max for 2.5 seconds
    Fan->setSpeed(i);
    delay(2500);
  }
  for (i=255; i!=0; i--) { //slows fan to stop
    Fan->setSpeed(i);
    delay(10);
  }

}

void fan_on5() { // turns fan on for 1 second . this works in dc motor test, but not in here
  uint8_t i;

  // Serial.print("fan on 5"); // uncomment for debugging to makesure function is getting called

  Fan->run(FORWARD); //turns fan on
  for (i=254; i<255; i++) { //sets speed to max for 1 second
    Fan->setSpeed(i);
    delay(1000);
  }
  for (i=255; i!=0; i--) { //slows fan to off
    Fan->setSpeed(i);
    delay(10);
  }

}



void fan_control() { // litterally its name
  uint8_t i;
  if (b == 1) {
    fan_on1(); 
    //Serial.print("Fan case = 1"); //for debugging
    //Serial.println(" ");
  }
  if (b == 2) {
    fan_on2();
    //Serial.print("Fan case = 2"); //for debugging
    //Serial.println(" ");
  }
  if (b == 3) {
    fan_on3();
    //Serial.print("Fan case = 3"); //for debugging
    //Serial.println(" ");
  }
  if (b == 4) {
    fan_on4();
    //Serial.print("Fan case = 4"); //for debugging
    //Serial.println(" ");
  }
  if (b == 5) {
    fan_on5();
    //Serial.print("Fan case = 5"); //for debugging
    //Serial.println(" ");
  }
  if (b == 6) {
    Fan->run(RELEASE); //turns fan off
    Fan->setSpeed(0);
    delay(1000);
    //Serial.print("Fan case = 6");
    //Serial.println(" ");
  }
  readtemp(); // update temps after fan cylce
}

void loop() {

  readtemp(); // start out by reading temps
  button_read(); // next read button value
  lcd_case_counter(); // determine if right or left is pressed to change screen
  if (a == 0) { // screen 1 - food setpoint
       lcd.clear(); // clear lcd screen
       lcd.setCursor(0,0); //sets print to top left - column 0, row 0
       lcd.print("Food Setpoint"); 
       lcd.setCursor(0,1); //sets print to bottom left - colum 0, row 1
       lcd.print(foodset); //prints the food setpoint variable - starts at 150
       button_read(); // reads for button press
       if (y == 2) { //if up pressed, increase foodset
          foodset++; // increase foodset by 1
          y = 0; //reset button value to 0 before next button read
          delay(75); // small delay to prevent runaway button presses
       }
       else if (y == 3) { //if down is pressed, decrease foodset
          foodset--; //decrease foodset by 1
          y = 0; 
          delay(75);
       }
     }
  else if (a == 1) { // screen 2 - grill setpoint - works exactly like the previous screen
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
   else if (a == 2) { // ask to start controlling the grill basically, can still press left or right at this point to go back to setpoints
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Press Select to");
         lcd.setCursor(0,1);
         lcd.print("Run");
         button_read();
         if (y == 5) { // if select is pressed, enter a loop that displays the food temp reading and the grill temp reading while controlling the fan
           while (y != 3) { //making it loop here unless back/left is held through the end of delay
            button_read();
            readtemp(); // quick, possibly unneccessary updating of temperatures
            lcd.setCursor(0,0); 
            lcd.print("Food       Grill");
            lcd.setCursor(0,1); // displays food temp underneath "Food"
            lcd.print(foodtemp);
            lcd.setCursor(10,1); // displays grill temp underneath "Grill"
            lcd.print(grilltemp);
            fan_case_counter(); // setting fan case value b
            fan_control(); // supposed to turn on the fan and read the temp again
            delay(20000); //delay 20 seconds before updating temps and turning on fan control again. purpose is to prevent a runaway condition on the fire
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
