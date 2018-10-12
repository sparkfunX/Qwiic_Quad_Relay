/*
  Qwiic Quad Relay
  By: Elias Santistevan
  Sparkfun Electronics
  Date: October, 2018
  License: This code is public domain but you buy me a beer if you use this
  and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!

  This example demonstrates how to change the I2C address via software.
*/

#include <Wire.h>

#define RELAY_TOGGLE_ONE 0x01

byte qwiicRelayAddress = 109; //0x6D is the only available address for the Quad Relay
byte newAddress = 70; //Must be 0x08 <= newAddress <= 0x77

const uint8_t red_button = 2; 

void setup(void)
{
  Wire.begin();

  pinMode(red_button, INPUT_PULLUP); 
  
  Serial.begin(9600);
  Serial.println("Qwiic Quad Relay Address Change.");

  // Address will change on startup. 
  // The new address must be 0x08 <= address <= 0x77 
  if(changeQuadRelayAddress(qwiicRelayAddress, newAddress) == true) //Old address, new address
  {
    qwiicRelayAddress = newAddress;
    Serial.println("Address successfully changed to:" + newAddress);
  }
}

void loop(void)
{
  //Let's test out the new address, press a button and turn on relay one.
  if(digitalRead(red_button) == LOW) {
    Wire.beginTransmission(qwiicRelayAddress);
    Wire.write(0x01); 
    Wire.endTransmission();
    delay(400);
  }
}

//Change the I2C address from one address to another
boolean changeQuadRelayAddress(byte oldAddress, byte newAddress)
{
  Wire.beginTransmission(oldAddress); //Communicate using the old address
  Wire.write(0xC7); //0xC7 is the register location on the Qwiic Quad Relay to change its I2C address
  Wire.write(newAddress); //Go to the new address
  if (Wire.endTransmission() != 0)
  {
    //Sensor did not ACK
    Serial.println("Error: Sensor did not ack");
    return(false);
  }
  return(true);
}

