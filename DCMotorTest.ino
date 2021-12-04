
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();



Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();

  
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);

  myMotor->run(RELEASE);
}

void fan_on() {
  uint8_t i;

  Serial.print("tick");

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

int y;

void loop(){
  y = 2;
  while (y != 1) {
  fan_on();
  }
}
