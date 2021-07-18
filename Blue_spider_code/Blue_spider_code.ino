#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define USMIN 350 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2500 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


#define LEG1_1 0
#define LEG1_2 1
#define LEG1_3 2

#define LEG2_1 4
#define LEG2_2 5
#define LEG2_3 6

#define LEG3_1 8
#define LEG3_2 9
#define LEG3_3 10

#define LEG4_1 12
#define LEG4_2 13
#define LEG4_3 14

float a1 = 48;
float b1 = 85;

void setServoDeg(uint8_t servonum, double deg) {
  double pulse = USMIN + ((USMAX-USMIN)/180) * deg;
  pwm.writeMicroseconds(servonum, pulse);
}

void legPos(int legNr, int jointDeg1, int jointDeg2, int jointDeg3) {
  int leg = -1;

  //Putting in right base address for servos
  if(legNr == 1) {
    leg = 0;
    jointDeg3 = 180-jointDeg3;
  }
  else if (legNr == 2) {
    leg = 4;
    jointDeg1 = 180-jointDeg1;
    jointDeg2 = 180-jointDeg2;
  }
  else if (legNr == 3) {
    leg = 8;
    jointDeg1 = 180-jointDeg1;
    jointDeg2 = 180-jointDeg2;
  }
  else if (legNr == 4) {
    leg = 12;
    jointDeg3 = 180-jointDeg3;
  }

  // Taking action
  if(leg !=-1) {
    setServoDeg(leg, jointDeg1);
    setServoDeg(leg+1, jointDeg2);
    setServoDeg(leg+2, jointDeg3);
  } else {
    Serial.println("Wrong leg number!");
  }
}

void move_z_axis(int z) {
  Serial.println(z);
  float insidecos = (a1*a1+b1*b1-z*z)/(2*a1*b1);
  double beta = acos(insidecos)*180/3.14;
  double alpha = (180-beta)/2;
  Serial.print("Beta: ");
  Serial.print(beta);
  Serial.print(" Alpha: "); 
  Serial.println(alpha);
  for(int i = 1; i <= 4; i++) {
    legPos(i, 45, int(beta), int(alpha));
  }

}

void restPos() {
  for(int i = 1; i <= 4; i++) {
    legPos(i, 45, 10, 90);
  }
}

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  restPos();

  delay(500);
}

void loop() {
  for(int i = 50; i<=120; i++) {
    move_z_axis(i);
    delay(100);
  }
  delay(500);
  for(int i = 120; i>=50; i--) {
    move_z_axis(i);
    delay(100);
  }
  delay(500);
}
