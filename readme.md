# 3-IN-1 Robotic Car with Arduino Nano
<p align="center">
  <img src="images/cover.jpg"/>
</p>

## Introduction
### About this project
My first Arduino project. This is a four-wheel car with 3 modes:
- Auto: Obstacle avoiding
- Manual: Bluetooth controlling
- Manual: Infrared controlling

Unlike many tutorials on the internet, I build my robot to stop automatically without having to press any `stop button`. If I want the robot to run, I'll just hold the button on the controller and release it when i don't want to. Therefore, the robot become easier to control o.<

### DEMO
#### Obstacle Avoiding
<p align="center">
  <img src="images/DEMO/obstacle_avoiding.gif"/>
</p>

#### Bluetooth Controlling
<p align="center">
  <img src="images/DEMO/bluetooth_controlling.gif"/>
</p>

#### IR Controlling
<p align="center">
  <img src="images/DEMO/IR_controlling.gif"/>
</p>

## Components and Supplies
| Image                                      | Name                        | QTY |
|--------------------------------------------|-----------------------------|-----|
| ![](images/components/arduino_nano.png)    | Arduino Nano                | 1   |
| ![](images/components/L298N.png)           | L298N - Motor Driver        | 1   |
| ![](images/components/DC_motor.png)        | 3-6V DC motor and wheel     | 4   |
| ![](images/components/SG90.png)            | SG90 - Micro Servo Motor    | 1   |
| ![](images/components/HC_SR04.png)         | HC-SR04 - Ultrasonic Sensor | 1   |
| ![](images/components/HC_06.png)           | HC-06 - Bluetooth module    | 1   |
| ![](images/components/KY_022.png)          | KY-022 - IR Receiver Sensor | 1   |
| ![](images/components/mini_breadboard.png) | Mini breadboard             | 1   |
| ![](images/components/box.png)             | Plastic box                 | 1   |

## Necessary tools
| Image                                      | Name                    |
|--------------------------------------------|-------------------------|
| ![](images/components/engineering_kit.png) | Kid's engineering kit   |
| ![](images/components/soldering_iron.png)  | Soldering iron          |
| ![](images/components/glue_gun.png)        | Hot glue gun            |
| ![](images/components/jumper_wires.png)    | Jumper wires            |
| ![](images/components/IRremote.png)        | Infrared remote control |

Notes:
- I only use the short L-shape bracket in the engineering kit. You can buy a mounting bracket for ultrasonic sensor, which is way better :3
- You can use any IR remote, just make sure to edit the buttons' hex value in the code before uploading it to Arduino. I use the remote shown above and its hex codes are:
```c
#define ON    0xFFA25D
#define OFF   0xFFE21D
#define MODE1 0xFFE01F
#define MODE2 0xFFA857
#define MODE3 0xFF906F
#define SOS   0xFF18E7
#define HOLD  0xFFFFFFFF // Generally, remote will send this value when you're holding a button
```
### How to exactly know the hex codes?
Connect IR receiver module with Arduino like this

<img src="images/IRremote_bb.svg"/>

Then plug in the USB cable to upload the code. 
```c
#include <IRremote.h>

#define RECV_PIN 2 // Specifying the pin connected to receiver module
IRrecv irrecv(RECV_PIN);
decode_results button;

void setup(){
	Serial.begin(9600);
	irrecv.enableIRIn(); // Initialization to receive IR signals 
}

void loop(){
	if(irrecv.decode(&button)){ // Store decoded signal in `button`, return 0 if nothing is received		
		Serial.println(button.value); // Print the value to the serial monitor
		irrecv.resume();
	}
}
```
On Arduino IDE, go to `Tools` -> `Serial Monitor` (or press Ctrl+Shift+M) then press the buttons on the remote respectively to get their Hex values on the monitor.

## App
- Arduino IDE
- Any Arduino bluetooth controller app. 

  I chose [Arduino Bluetooth Controlled Joystick](https://play.google.com/store/apps/details?id=uncia.robotics.joystick). Besides two joystick modes and one accelerometer mode, this app provides `Button Mode` - the mode i'm using in this project. I use the default setting in the app:
  
  | Button      | Symbol | Character send |
  |:-----------:|:------:|----------------|
  | Up arrow    | ˄      | N              |
  | Down arrow  | ˅      | S              |
  | Left arrow  | ˂      | W              |
  | Right arrow | ˃      | E              |
  | Cross       | X      | 1              |
  | Circle      | O      | 2              |
  | Triangle    | Δ      | 3              |
  | Square      | □      | 4              |

## Schematic
![](images/3_in_1_robot_bb.svg)
