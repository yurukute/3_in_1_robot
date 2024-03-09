#include <Servo.h>
#include <NewPing.h>        // Ultrasonic sensor
#include <IRremote.h> 
#include <SoftwareSerial.h> // Bluetooth communication

// Directions
#define STRAIGHT 100
#define LEFT     180
#define RIGHT    10
// IRremote's buttons
#define ON    0xFFA25D
#define OFF   0xFFE21D
#define MODE1 0xFFE01F
#define MODE2 0xFFA857
#define MODE3 0xFF906F
#define SOS   0xFF18E7
#define HOLD  0xFFFFFFFF
// Pins
#define RX   0
#define TX   1
#define ECHO 2
#define TRIG 3

SoftwareSerial BTserial(RX, TX); 

NewPing sonar(TRIG, ECHO, 250);

Servo servo;

IRrecv irrecv(9);
decode_results button;

const int motors[] = {7,6,5,4}; //L298N pins
int distance = 100, mode = 0;
char last_command;

void setup() {
  for(int i : motors) pinMode(i, OUTPUT);
  servo.attach(8);
  servo.write(STRAIGHT);
  irrecv.enableIRIn();
  Serial.begin(9600);
}

void loop() {
  if(mode != 3 && irrecv.decode(&button)){
    switchMode(button.value);
    irrecv.resume();
  }
  if(mode != 2 && Serial.available()){    
    switchMode(Serial.read());    
  }
  switch(mode){
  case 0: // Stanby mode
    stop();
    servo.write(STRAIGHT);
    break;
  case 1: obstacleAvoiding();     break;
  case 2: bluetoothControlling(); break;    
  case 3: irControlling();        break;
  }
}

void switchMode(int data){
  switch(data){
  case ON:
    if(mode == 0)
      mode = 1;
    break;
  case OFF: case '4':
    mode = 0;
    break;
  case MODE1: case '1':
    mode = 1; 
    break;
  case MODE2: case '2':
    mode = 2;
    Serial.read();
    break;
  case MODE3: case '3':
    mode = 3;
    break;    
  } 
}

void obstacleAvoiding(){
  if(distance < 30){
    stop();
    delay(300);
    moveBackward();
    delay(400);
    stop();
    delay(300);
    getDistance(LEFT) > getDistance(RIGHT) ? turnLeft() : turnRight();
    delay(400);
    stop();
  }
  else moveForward(); 
  distance = getDistance(STRAIGHT);
}

int getDistance(int DIRECTION){
  servo.write(DIRECTION);
  delay(70);
  int newDistance = sonar.ping_cm();
  if(DIRECTION != STRAIGHT){
    delay(500);
    servo.write(STRAIGHT);
  }
  return newDistance == 0 ? 250 : newDistance;  
}

void bluetoothControlling(){
  if(Serial.available()){
    switch(Serial.read()){
    case '1':
      switchMode(OFF);
      Serial.read();
      break;
    case '2': servo.write(RIGHT);     break;      
    case '3': servo.write(STRAIGHT);  break;
    case '4': servo.write(LEFT);      break;
    case 'N': moveForward();          break;
    case 'S': moveBackward();         break;
    case 'W': turnLeft();             break;
    case 'E': turnRight();            break;
    }
  }
  delay(60);
  stop();
}

void irControlling(){
  if(irrecv.decode(&button)){     
    translateIR(button.value);
    irrecv.resume();
  }
  delay(200); 
  stop();
}

void translateIR(int data){
  switch(data){
  case OFF:
    switchMode(OFF);
    break;
  case MODE1: case 'L':
    turnLeft();
    last_command = 'L';
    break;
  case MODE2: case 'F':
    moveForward();
    last_command = 'F';
    break;       
  case MODE3: case 'R':
    turnRight();
    last_command = 'R';
    break;          
  case SOS: case 'B':
    moveBackward();
    last_command = 'B';
    break;
  case HOLD:
    translateIR(last_command);
    break;
  }
}

void stop(){
  for(int i : motors){
    digitalWrite(i, LOW);
  }
}

void moveForward(){
  digitalWrite(motors[0], HIGH); //IN1
  digitalWrite(motors[1], LOW);  //IN2
  digitalWrite(motors[2], HIGH); //IN3
  digitalWrite(motors[3], LOW);  //IN4
}

void moveBackward(){
  digitalWrite(motors[0], LOW);
  digitalWrite(motors[1], HIGH);
  digitalWrite(motors[2], LOW);
  digitalWrite(motors[3], HIGH);
}

void turnLeft(){
  digitalWrite(motors[0], HIGH);
  digitalWrite(motors[1], LOW);
  digitalWrite(motors[2], LOW);
  digitalWrite(motors[3], HIGH); 
}

void turnRight(){
  digitalWrite(motors[0], LOW);
  digitalWrite(motors[1], HIGH);
  digitalWrite(motors[2], HIGH);
  digitalWrite(motors[3], LOW);  
}
