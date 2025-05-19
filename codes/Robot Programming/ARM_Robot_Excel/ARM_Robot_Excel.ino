/* This code for MeArm robot using joystick shield with arduino mega
   6 DOF
   Please use orginal arduino Uno
   I use PCA9685
   Copy the library to libraries folder
*/
/* Include the HCPCA9685 library */
#include "HCPCA9685.h"
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

//Incoming Serial Data Array
const byte channel = 7;
char* arr[channel];
String speed_move;

// Declare the pins for the Button
int x1_pin = 2;
int x2_pin = 3;
int y1_pin = 4;
int y2_pin = 5;
int z11_pin = 6;
int z12_pin = 7;
int z21_pin = 8;
int z22_pin = 9;
int rotate1_pin = 10;
int rotate2_pin = 11;
int grip1_pin = 12;
int grip2_pin = 13;

int x_pos;       // variable to store the servo position
int y_pos;       // variable to store the servo position
int z1_pos;      // variable to store the servo position
int z2_pos;      // variable to store the servo position
int rotate_pos;  // variable to store the servo position
int grip_pos;    // variable to store the servo position

int x_target;
int y_target;
int z1_target;
int z2_target;
int rotate_target;
int grip_target;

void setup() {
  //button
  pinMode(x1_pin, INPUT_PULLUP);
  pinMode(x2_pin, INPUT_PULLUP);
  pinMode(y1_pin, INPUT_PULLUP);
  pinMode(y2_pin, INPUT_PULLUP);
  pinMode(z11_pin, INPUT_PULLUP);
  pinMode(z12_pin, INPUT_PULLUP);
  pinMode(z21_pin, INPUT_PULLUP);
  pinMode(z22_pin, INPUT_PULLUP);
  pinMode(rotate1_pin, INPUT_PULLUP);
  pinMode(rotate2_pin, INPUT_PULLUP);
  pinMode(grip1_pin, INPUT_PULLUP);
  pinMode(grip2_pin, INPUT_PULLUP);

  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);

  //origin position
  x_pos = 238;
  y_pos = 120;
  z1_pos = 0;
  z2_pos = 400;
  rotate_pos = 0;
  grip_pos = 220;

  HCPCA9685.Servo(0, x_pos);
  HCPCA9685.Servo(1, y_pos);
  HCPCA9685.Servo(2, z1_pos);
  HCPCA9685.Servo(3, z2_pos);
  HCPCA9685.Servo(4, rotate_pos);
  HCPCA9685.Servo(5, grip_pos);
  print_position();
}

void loop() {
  delay(30);
  //button check--------------------------------------------
  int buttonValueX1 = digitalRead(x1_pin);
  int buttonValueX2 = digitalRead(x2_pin);
  int buttonValueY1 = digitalRead(y1_pin);
  int buttonValueY2 = digitalRead(y2_pin);
  int buttonValueZ11 = digitalRead(z11_pin);
  int buttonValueZ12 = digitalRead(z12_pin);
  int buttonValueZ21 = digitalRead(z21_pin);
  int buttonValueZ22 = digitalRead(z22_pin);
  int buttonValueRotate1 = digitalRead(rotate1_pin);
  int buttonValueRotate2 = digitalRead(rotate2_pin);
  int buttonValueGrip1 = digitalRead(grip1_pin);
  int buttonValueGrip2 = digitalRead(grip2_pin);
  //button check--------------------------------------------

  //For auto movement*********************************************
  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();

  //For auto movement*********************************************

  // button X========================================
  if (buttonValueX1 == LOW) {
    if (x_pos < 450) {
      x_pos = x_pos + 2;
      HCPCA9685.Servo(0, x_pos);
      print_position();
    }
  }
  if (buttonValueX2 == LOW) {
    if (x_pos > 0) {
      x_pos = x_pos - 2;
      HCPCA9685.Servo(0, x_pos);
      print_position();
    }
  }
  // button X========================================

  // button Y========================================
  if (buttonValueY1 == LOW) {
    if (y_pos < 450) {
      y_pos = y_pos + 2;
      HCPCA9685.Servo(1, y_pos);
      print_position();
    }
  }
  if (buttonValueY2 == LOW) {
    if (y_pos > 0) {
      y_pos = y_pos - 2;
      HCPCA9685.Servo(1, y_pos);
      print_position();
    }
  }
  // button Y========================================

  // button Z1========================================
  if (buttonValueZ11 == LOW) {
    if (z1_pos < 450) {
      z1_pos = z1_pos + 2;
      HCPCA9685.Servo(2, z1_pos);
      print_position();
    }
  }
  if (buttonValueZ12 == LOW) {
    if (z1_pos > 0) {
      z1_pos = z1_pos - 2;
      HCPCA9685.Servo(2, z1_pos);
      print_position();
    }
  }
  // button Z1========================================

  // button Z2========================================
  if (buttonValueZ21 == LOW) {
    if (z2_pos < 450) {
      z2_pos = z2_pos + 2;
      HCPCA9685.Servo(3, z2_pos);
      print_position();
    }
  }
  if (buttonValueZ22 == LOW) {
    if (z2_pos > 0) {
      z2_pos = z2_pos - 2;
      HCPCA9685.Servo(3, z2_pos);
      print_position();
    }
  }
  // button Z2========================================

  // button rotate========================================
  if (buttonValueRotate1 == LOW) {
    if (rotate_pos < 450) {
      rotate_pos = rotate_pos + 2;
      HCPCA9685.Servo(4, rotate_pos);
      print_position();
    }
  }
  if (buttonValueRotate2 == LOW) {
    if (rotate_pos > 0) {
      rotate_pos = rotate_pos - 2;
      HCPCA9685.Servo(4, rotate_pos);
      print_position();
    }
  }
  // button rotate========================================

  // button grip========================================
  if (buttonValueGrip1 == LOW) {
    if (grip_pos < 220) {
      grip_pos = grip_pos + 2;
      HCPCA9685.Servo(5, grip_pos);
      print_position();
    }
  }
  if (buttonValueGrip2 == LOW) {
    if (grip_pos > 100) {
      grip_pos = grip_pos - 2;
      HCPCA9685.Servo(5, grip_pos);
      print_position();
    }
  }
  // button grip========================================
}

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if (Serial.available()) {
    parseData(GetSerialData());
    //Get data from serial and send to move function of servo
    x_target = atoi(arr[0]);
    y_target = atoi(arr[1]);
    z1_target = atoi(arr[2]);
    z2_target = atoi(arr[3]);
    rotate_target = atoi(arr[4]);
    grip_target = atoi(arr[5]);
    speed_move = String(arr[6]);
    if (speed_move=="Normal"){
      move_normal();
    }
    else {
      move_slow();
    }
  }
}

// Gathers bytes from serial port to build inputString
//char* GetSerialData()
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()) {
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  return inputString;
}

// Seperate the data at each delimeter
void parseData(char data[])
{
  char *token = strtok(data, ","); // Find the first delimeter and return the token before it
  int index = 0; // Index to track storage in the array
  while (token != NULL) { // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
    arr[index] = token; // Assign the token to an array
    token = strtok(NULL, ","); // Conintue to the next delimeter
    index++; // incremenet index to store next value
  }
}

void print_position() {
  //send data serial to excel
  //Format: X,Y,Z1,Z2,Rotate,Grip
  Serial.print(x_pos);
  Serial.print(",");
  Serial.print(y_pos);
  Serial.print(",");
  Serial.print(z1_pos);
  Serial.print(",");
  Serial.print(z2_pos);
  Serial.print(",");
  Serial.print(rotate_pos);
  Serial.print(",");
  Serial.print(grip_pos);
  Serial.println();
}


void home_position() {
  x_target = 238;
  y_target = 120;
  z1_target = 0;
  z2_target = 400;
  rotate_target = 0;
  grip_target = 220;
  move_normal();
}

void move_normal() {
  for (int i = 0; i < 450; i++) {
    delay(20);  // Delay for smooth movement
    //Move axis x-----------------------------
    if (x_pos < x_target) {
      x_pos = x_pos + 2;
      HCPCA9685.Servo(0, x_pos);
    }
    if (x_pos > x_target) {
      x_pos = x_pos - 2;
      HCPCA9685.Servo(0, x_pos);
    }
    //Move axis x-----------------------------

    //Move axis y-----------------------------
    if (y_pos < y_target) {
      y_pos = y_pos + 2;
      HCPCA9685.Servo(1, y_pos);
    }
    if (y_pos > y_target) {
      y_pos = y_pos - 2;
      HCPCA9685.Servo(1, y_pos);
    }
    //Move axis y-----------------------------

    //Move axis z1-----------------------------
    if (z1_pos < z1_target) {
      z1_pos = z1_pos + 2;
      HCPCA9685.Servo(2, z1_pos);
    }
    if (z1_pos > z1_target) {
      z1_pos = z1_pos - 2;
      HCPCA9685.Servo(2, z1_pos);
    }
    //Move axis z1-----------------------------

    //Move axis z2-----------------------------
    if (z2_pos < z2_target) {
      z2_pos = z2_pos + 2;
      HCPCA9685.Servo(3, z2_pos);
    }
    if (z2_pos > z2_target) {
      z2_pos = z2_pos - 2;
      HCPCA9685.Servo(3, z2_pos);
    }
    //Move axis z2-----------------------------
    //Move axis rotate-----------------------------
    if (rotate_pos < rotate_target) {
      rotate_pos = rotate_pos + 2;
      HCPCA9685.Servo(4, rotate_pos);
    }
    if (rotate_pos > rotate_target) {
      rotate_pos = rotate_pos - 2;
      HCPCA9685.Servo(4, rotate_pos);
    }
    //Move axis rotate-----------------------------

    //Move axis grip-----------------------------
    if (grip_pos < grip_target) {
      grip_pos = grip_pos + 2;
      HCPCA9685.Servo(5, grip_pos);
    }
    if (grip_pos > grip_target) {
      grip_pos = grip_pos - 2;
      HCPCA9685.Servo(5, grip_pos);
    }
    //Move axis grip-----------------------------
    if (x_pos == x_target && y_pos == y_target && z1_pos == z1_target && z2_pos == z2_target && rotate_pos == rotate_target && grip_pos == grip_target) {
      break;
    }
  }
  print_position();
}

void move_slow() {
  for (int i = 0; i < 450; i++) {
    delay(30);  // Delay for smooth movement
    //Move axis x-----------------------------
    if (x_pos < x_target) {
      x_pos = x_pos + 1;
      HCPCA9685.Servo(0, x_pos);
    }
    if (x_pos > x_target) {
      x_pos = x_pos - 1;
      HCPCA9685.Servo(0, x_pos);
    }
    //Move axis x-----------------------------

    //Move axis y-----------------------------
    if (y_pos < y_target) {
      y_pos = y_pos + 1;
      HCPCA9685.Servo(1, y_pos);
    }
    if (y_pos > y_target) {
      y_pos = y_pos - 1;
      HCPCA9685.Servo(1, y_pos);
    }
    //Move axis y-----------------------------

    //Move axis z1-----------------------------
    if (z1_pos < z1_target) {
      z1_pos = z1_pos + 1;
      HCPCA9685.Servo(2, z1_pos);
    }
    if (z1_pos > z1_target) {
      z1_pos = z1_pos - 1;
      HCPCA9685.Servo(2, z1_pos);
    }
    //Move axis z1-----------------------------

    //Move axis z2-----------------------------
    if (z2_pos < z2_target) {
      z2_pos = z2_pos + 2;
      HCPCA9685.Servo(3, z2_pos);
    }
    if (z2_pos > z2_target) {
      z2_pos = z2_pos - 2;
      HCPCA9685.Servo(3, z2_pos);
    }
    //Move axis z2-----------------------------
    //Move axis rotate-----------------------------
    if (rotate_pos < rotate_target) {
      rotate_pos = rotate_pos + 1;
      HCPCA9685.Servo(4, rotate_pos);
    }
    if (rotate_pos > rotate_target) {
      rotate_pos = rotate_pos - 1;
      HCPCA9685.Servo(4, rotate_pos);
    }
    //Move axis rotate-----------------------------

    //Move axis grip-----------------------------
    if (grip_pos < grip_target) {
      grip_pos = grip_pos + 1;
      HCPCA9685.Servo(5, grip_pos);
    }
    if (grip_pos > grip_target) {
      grip_pos = grip_pos - 1;
      HCPCA9685.Servo(5, grip_pos);
    }
    //Move axis grip-----------------------------
    if (x_pos == x_target && y_pos == y_target && z1_pos == z1_target && z2_pos == z2_target && rotate_pos == rotate_target && grip_pos == grip_target) {
      break;
    }
  }
  print_position();
}
