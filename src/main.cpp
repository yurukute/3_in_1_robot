#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>        // Ultrasonic sensor
#include <IRremote.hpp>     // IRremote v4.x
#include <SoftwareSerial.h> // Bluetooth communication

// Directions
#define STRAIGHT 100
#define LEFT     180
#define RIGHT    10

// IRremote's buttons
#define ON    69
#define OFF   71
#define MODE1 7
#define MODE2 21
#define MODE3 9
#define SOS   24

// Pins
#define RX   0
#define TX   1
#define ECHO 2
#define TRIG 3
#define IN1  7
#define IN2  6
#define IN3  5
#define IN4  4

// Modes
#define Inactive 0
#define ObsAvoid 1
#define BTCtrlng 2
#define IRCtrlng 3

SoftwareSerial BTserial(RX, TX);
NewPing sonar(TRIG, ECHO, 250);
Servo servo;
IRrecv irrecv(9);
decode_results button;

const int motors[] = {IN1, IN2, IN3, IN4}; //L298N pins
int distance = 100;
int mode = Inactive;

void switchMode(int signal);
void obstacleAvoiding();
void bluetoothControlling();
void irControlling();
void stop();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void getDistance();

void setup() {
    for(int i : motors) pinMode(i, OUTPUT);
    servo.attach(8);
    servo.write(STRAIGHT);
    irrecv.enableIRIn();
    Serial.begin(9600);
}

void loop() {
    if(mode != 3 && irrecv.decode()){
        switchMode(irrecv.decodedIRData.command);
        irrecv.resume();
    }
    if(mode != 2 && Serial.available()){
        switchMode(Serial.read());
    }
    switch(mode){
    case Inactive:
        stop();
        servo.write(STRAIGHT);
        break;
    case ObsAvoid: obstacleAvoiding();     break;
    case BTCtrlng: bluetoothControlling(); break;
    case IRCtrlng: irControlling();        break;
    }
}

void switchMode(int signal){
    switch(signal){
    case ON:
        if(mode == 0)
            mode = 1;
        break;
    case OFF:   case Inactive: mode = Inactive; break;
    case MODE1: case ObsAvoid: mode = ObsAvoid; break;
    case MODE2: case BTCtrlng: mode = BTCtrlng; break;
    case MODE3: case IRCtrlng: mode = IRCtrlng; break;
    }
    Serial.println(String(signal) + " received. Mode:" + mode);
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

void processSignal(int data){
    switch(data){
    case OFF:             switchMode(OFF); break;
    case MODE1: case 'L': turnLeft();      break;
    case MODE2: case 'F': moveForward();   break;
    case MODE3: case 'R': turnRight();     break;
    case SOS:   case 'B': moveBackward();  break;
    }
}

void irControlling(){
    if(irrecv.decode()){
        processSignal(irrecv.decodedIRData.command);
        irrecv.resume();
    }
    delay(200); 
    stop();
}

void stop(){
    for(int i : motors){
        digitalWrite(i, LOW);
    }
}

void moveForward(){
    Serial.println("Moving fordward");
    digitalWrite(motors[0], HIGH); //IN1
    digitalWrite(motors[1], LOW);  //IN2
    digitalWrite(motors[2], HIGH); //IN3
    digitalWrite(motors[3], LOW);  //IN4
}

void moveBackward(){
    Serial.println("Moving backward");
    digitalWrite(motors[0], LOW);
    digitalWrite(motors[1], HIGH);
    digitalWrite(motors[2], LOW);
    digitalWrite(motors[3], HIGH);
}

void turnLeft(){
    Serial.println("Turning left.");
    digitalWrite(motors[0], HIGH);
    digitalWrite(motors[1], LOW);
    digitalWrite(motors[2], LOW);
    digitalWrite(motors[3], HIGH); 
}

void turnRight(){
    Serial.println("Turning right.");
    digitalWrite(motors[0], LOW);
    digitalWrite(motors[1], HIGH);
    digitalWrite(motors[2], HIGH);
    digitalWrite(motors[3], LOW);  
}
