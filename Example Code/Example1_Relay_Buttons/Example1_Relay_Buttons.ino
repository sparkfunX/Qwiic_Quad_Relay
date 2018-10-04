/*
  This Qwiic Quad relay is a product with four channels for switching high AC voltge.
  By: Elias Santistevan
  SparkFun Electronics
  Date: October 2018

  License: This code is public domain but you buy me a beer if you use 
	this and we meet someday (Beerware license).

	This example attaches four buttons to four digital I/O pins and each controls
  a different relay.  When a button is pressed it sends and I2C command indicating 
  which channel on the relay board to turn on. This example also includes a
  function to check whether the relays are on or off.  

*/

#include <Wire.h>

#define RELAY_ADDR 0x6D //Only address that exists for this product

// Here are the commands to turn on and off individual relays. 
#define TOGGLE_RELAY_ONE 0x01
#define TOGGLE_RELAY_TWO 0x02
#define TOGGLE_RELAY_THREE 0x03
#define TOGGLE_RELAY_FOUR 0x04

// Here are the commands to turn them all off or on. 
#define TURN_ALL_ON  0xB 
#define TURN_ALL_OFF 0xA

// Here are the commands to check on the 'status' of the relay i.e. whether the
// relay is on or off.
#define RELAY_ONE_STATUS 0x05
#define RELAY_TWO_STATUS 0x06
#define RELAY_THREE_STATUS 0x07
#define RELAY_FOUR_STATUS 0x08

//Four buttons
const uint8_t yellow_btn = 2;
const uint8_t blue_btn = 3;
const uint8_t red_btn =  4;
const uint8_t green_btn =  5;

void setup()
{
  Wire.begin(); 
  Serial.begin(115200);
	
	//Use internal resitors to keep them in a known high state. 
  pinMode(yellow_btn, INPUT_PULLUP);
  pinMode(blue_btn, INPUT_PULLUP);
  pinMode(red_btn, INPUT_PULLUP);
  pinMode(green_btn, INPUT_PULLUP);

  get_relays_status();
}

void loop()
{
	// Since we'll only ever want the relay to be on or off,
	// the logic is handled by the product. Here we're just pressing buttons and
  // putting a small 400 ms debounce. 

	//button one, relay one!
  if(digitalRead(yellow_btn) == LOW){
    Serial.println("Yellow Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TOGGLE_RELAY_ONE);
    Wire.endTransmission();
		delay(400);
  }

	//button two, relay two!
  if(digitalRead(blue_btn) == LOW){
    Serial.println("Blue Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TOGGLE_RELAY_TWO);
    Wire.endTransmission();
		delay(400);
  }

	//button three, relay three!
  if(digitalRead(red_btn) == LOW){
    Serial.println("Red Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TURN_ALL_ON);
    Wire.endTransmission();
		delay(400);
  }

	//button four, relay four!
  if(digitalRead(green_btn) == LOW){
    Serial.println("Green Button");
    Wire.beginTransmission(RELAY_ADDR);
    Wire.write(TURN_ALL_OFF);
    Wire.endTransmission();
		delay(400);
  }
}

void get_relays_status(){
  int i = 1; 
  Wire.beginTransmission(RELAY_ADDR);
  // Change the Wire.write statment below to the desired relay you want to check on.
  Wire.write(RELAY_ONE_STATUS); 
  Wire.endTransmission();
  //  We want the states of all the relays so we're requesting four bytes, i.e.
  //  four relays. If you request 2 bytes, it will give you two relays! 
  Wire.requestFrom(RELAY_ADDR, 4); 


  // Print it out, 0 == OFF and 15 == ON.  
  while(Wire.available()){
    Serial.print("Relay"); 
    Serial.print(" i:"); 
    Serial.println(Wire.read());
  }

}
