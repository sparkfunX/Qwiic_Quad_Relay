/*
  The Qwiic Quad Relay product is relay board with four channels for switching high AC voltge.
  By: Elias Santistevan
  SparkFun Electronics
  Date: October 2018

  License: This code is public domain but you buy me a beer if you use
  this and we meet someday (Beerware license).

  This example uses two buttons attached to the only two hardware interrupt pins 
  on an Arduino Uno. If you're using a 32u4 based product that you have access
  to five interrupts. When a button is pressed it triggers and interrupt which
  sends an I2C command to the Qwiic Quad Relay. There's also a function to
  check whether the relay is on or off. 

*/




#include <Wire.h>

// Only address that exists for this product
#define RELAY_ADDR 0x6D 

// Here are the commands to turn on and off individual relays.
// Only need two since we're using an Arduino Uno.  
#define TOGGLE_RELAY_ONE 0x01
#define TOGGLE_RELAY_TWO 0x02
//#define TOGGLE_RELAY_THREE 0x03
//#define TOGGLE_RELAY_FOUR 0x04

// Here are the commands to turn on or off all relay channels. 
//#define TURN_ALL_ON  0xB
//#define TURN_ALL_OFF 0xA

// Here are the commands to check on the 'status' of the relay
// i.e. whether the relay is on or off.
#define RELAY_ONE_STATUS 0x05
#define RELAY_TWO_STATUS 0x06
//#define RELAY_THREE_STATUS 0x07
//#define RELAY_FOUR_STATUS 0x08 

// The Uno only has two interrupt pins on 2 and 3. Anything with the 32u4 will
// have 5 interrupt pins: 0,1,2,3,7. 
const uint8_t yellow_btn = 2;
const uint8_t blue_btn = 3;
//const uint8_t red_btn =  0;
//const uint8_t green_btn =  1;

// The flags are indicators that a button was pressed.
// When used this way with the interrupts, I don't have to worry
// about the interrupt being missed. 
volatile uint8_t yellowFlag = 0;
volatile uint8_t blueFlag = 0;
volatile uint8_t redFlag = 0;
volatile uint8_t greenFlag = 0;

// This will keep track of the state of the relay that is on or off. 
volatile uint8_t yellowState = 0;
volatile uint8_t blueState = 0;
volatile uint8_t redState = 0;
volatile uint8_t greenState = 0;

void setup()
{
  Wire.begin(RELAY_ADDR); 
  Serial.begin(115200);

  pinMode(yellow_btn, INPUT_PULLUP);
  pinMode(blue_btn, INPUT_PULLUP);
  //pinMode(red_btn, INPUT_PULLUP);
  //pinMode(green_btn, INPUT_PULLUP);

  // Since we're using buttons, we want to see when the button goes from high,
  // to low. In other words, when the button is pressed. If I were using a
  // switch, then I would change the FALLING parameter, to LOW. 
  attachInterrupt(digitalPinToInterrupt(yellow_btn), turn_off_yellow, FALLING);
  attachInterrupt(digitalPinToInterrupt(blue_btn), turn_off_blue, FALLING);
 // attachInterrupt(digitalPinToInterrupt(red_btn), turn_off_red, FALLING);
//  attachInterrupt(digitalPinToInterrupt(green_btn), turn_off_green, FALLING);
  
  get_relays_status()
  
}

void loop()
{
  // If the flag was tripped, then that means a button or switch was flipped.
  // The "state" tells us whether the relay is on or off. If one is flipped
  // than the other will be flipped to match. We then save the new 'state' of
  // the product.  
  if(yellowFlag != yellowState){
    yellowState = !yellowState; 
    Serial.println("Yellow Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TOGGLE_RELAY_ONE);
    Wire.endTranmission();
  }

  if(blueFlag != blueState){
    blueState = !blueState; 
    Serial.println("Blue Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TOGGLE_RELAY_ONE);
    Wire.endTranmission();
  }

//Uncomment for more buttons in the case you're using 32u4, mega, or anything
//with more interrupt lines.
//  if(digitalRead(red_btn) == LOW){
//    Serial.println("Red Button");
//    Wire.beginTransmission(RELAY_ADDR);
//    Wire.write(TOGGLE_RELAY_ONE);
//    Wire.endTranmission();
//  }

//  if(digitalRead(green_btn) == LOW){
//    Serial.println("Green Button");
//    Wire.beginTransmission(RELAY_ADDR);
//    Wire.write(TOGGLE_RELAY_ONE);
//    Wire.endTranmission();
//  }
//
}

// The following are interrupt service routine functions. Keep them super
// brief!
void turn_off_yellow()
{
  yellowFlag = !yellowFlag; 
}

void turn_off_blue()
{
  blueFlag = !blueFlag; 
}

void turn_off_red()
{
  redFlag = !redFlag; 
}

void turn_off_green()
{
  greenFlag = !greenFlag; 
}

void get_relays_status(){
  int i = 1;
  Wire.beginTransmission(RELAY_ADDR);
  // If you want to check a different relay then change the
  // Wire.write statment below to the relay you want to check.
  Wire.write(RELAY_ONE_STATUS);
  Wire.endTransmission();
  // We want the states of two relays so we're requesting two bytes. One byte
  // would get you....yeup one relay!
  Wire.requestFrom(RELAY_ADDR, 2);
  
  // Print it out, 0 == OFF and 15 == ON.
  while(Wire.available()){
   Serial.print("Relay");
   Serial.print(" i:");
   Serial.println(Wire.read());
 }
}

