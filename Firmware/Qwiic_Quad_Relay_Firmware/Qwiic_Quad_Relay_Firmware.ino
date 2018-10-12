/*
  An I2C based KeyPad
  By: Elias Santistevan
  SparkFun Electronics
  Date: October 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
	<Product Page Here>

  To install support for ATtiny84 in Arduino IDE: https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  This core is installed from the Board Manager menu
  This core has built in support for I2C S/M and serial
  If you have Dave Mellis' ATtiny installed you may need to remove it from \Users\xx\AppData\Local\Arduino15\Packages

  To support 400kHz I2C communication reliably ATtiny84 needs to run at 8MHz. This requires user to
  click on 'Burn Bootloader' before code is loaded.
*/

#include <Wire.h>

#include <EEPROM.h>

#define LOCATION_I2C_ADDRESS 0x01 //Location in EEPROM where the I2C address is stored
#define I2C_ADDRESS_DEFAULT 109 //0x6D Only one default address

#define COMMAND_CHANGE_ADDRESS 0xC7

// Commands that toggle the relays.
#define RELAY_ONE_TOGGLE 0x01
#define RELAY_TWO_TOGGLE 0x02
#define RELAY_THREE_TOGGLE 0x03
#define RELAY_FOUR_TOGGLE 0x04
#define TURN_ALL_OFF 0xA 
#define TURN_ALL_ON 0xB 
#define TOGGLE_ALL 0xC

// Commands to request the state of the relay, whether it is currently on or
// off. 
#define RELAY_STATUS_ONE  0x05 
#define RELAY_STATUS_TWO  0x06
#define RELAY_STATUS_THREE 0x07
#define RELAY_STATUS_FOUR 0x08

// Messages for requests that state whether a relay is on or off. 
#define RELAY_IS_ON 0xF
#define RELAY_IS_OFF 0x0

volatile uint8_t setting_i2c_address = I2C_ADDRESS_DEFAULT; 
volatile uint8_t COMMAND;  //Variable for incoming I2C commands. 

const uint8_t addrPin = 7; 
const uint8_t resetPin = 11; 

// The four relay pins on the ATtiny84A.
const uint8_t relayCtrl1 = 0; 
const uint8_t relayCtrl2 = 1; 
const uint8_t relayCtrl3 = 2; 
const uint8_t relayCtrl4 = 3; 

// This 'status' array keeps track of the state of the relays. 
uint8_t status[] = {RELAY_IS_OFF, RELAY_IS_OFF, RELAY_IS_OFF, RELAY_IS_OFF};

void setup(void)
{
  pinMode(addrPin, INPUT_PULLUP); //Default HIGH = 0x6D or 109
  pinMode(resetPin, INPUT_PULLUP);

  pinMode(relayCtrl1, OUTPUT); //Relay pins to OUTPUT
  pinMode(relayCtrl2, OUTPUT);
  pinMode(relayCtrl3, OUTPUT);
  pinMode(relayCtrl4, OUTPUT);

  readSystemSettings(); //Load all system settings from EEPROM

  startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
}

void loop(void) {
}

void receiveEvent(int numberOfBytesReceived)
{
  while( Wire.available() ) {
    //Record uint8_ts to local array
    COMMAND = Wire.read();

    if(COMMAND == COMMAND_CHANGE_ADDRESS){ //Set new I2C address 
      if(Wire.available()) {
        setting_i2c_address = Wire.read();

        //Error check
        if(setting_i2c_address < 0x08 || setting_i2c_address > 0x77)
          continue; //Command failed. This address is out of bounds.

        EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);

        //Our I2C address may have changed because of user's command
        startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
      }
    }

    // Relay control for relay 1, check the state, toggle the relay, change the
    // state.
    else if(COMMAND == RELAY_ONE_TOGGLE){
      if(status[0] == RELAY_IS_OFF){
        digitalWrite(relayCtrl1, HIGH); 
        status[0] = RELAY_IS_ON; 
      }
      else if(status[0] == RELAY_IS_ON ){
        digitalWrite(relayCtrl1, LOW); 
        status[0] = RELAY_IS_OFF; 
      }
    }

    // Relay control for relay 2, check the state, toggle the relay, change the
    // state.
    else if(COMMAND == RELAY_TWO_TOGGLE){
      if(status[1] == RELAY_IS_OFF){
        digitalWrite(relayCtrl2, HIGH); 
        status[1] = RELAY_IS_ON; 
      }
      else if(status[1] == RELAY_IS_ON ){
        digitalWrite(relayCtrl2, LOW); 
        status[1] = RELAY_IS_OFF; 
      }
    }

    // Relay control for relay 3, check the state, toggle the relay, change the
    // state.
    else if(COMMAND == RELAY_THREE_TOGGLE){
      if(status[2] == RELAY_IS_OFF){
        digitalWrite(relayCtrl3, HIGH); 
        status[2] = RELAY_IS_ON; 
      }
      else if(status[2] == RELAY_IS_ON ){
        digitalWrite(relayCtrl3, LOW); 
        status[2] = RELAY_IS_OFF; 
      }
    }

    // Relay control for relay 4, check the state, toggle the relay, change the
    // state.
    else if(COMMAND == RELAY_FOUR_TOGGLE){
      if(status[3] == RELAY_IS_OFF){
        digitalWrite(relayCtrl4, HIGH); 
        status[3] = RELAY_IS_ON; 
      }
      else if(status[3] == RELAY_IS_ON ){
        digitalWrite(relayCtrl4, LOW); 
        status[3] = RELAY_IS_OFF; 
      }
    }

    // Turn all relays off, record all of their states. 
    else if(COMMAND == TURN_ALL_OFF){
      digitalWrite(relayCtrl1, LOW); 
      digitalWrite(relayCtrl2, LOW); 
      digitalWrite(relayCtrl3, LOW); 
      digitalWrite(relayCtrl4, LOW); 
      for(int i = 0; i < sizeof(status); i++){
        status[i] = RELAY_IS_OFF;
      }
    }

    // Turn it up to ELEVEN, all on! Record their states to the status array.
    else if(COMMAND == TURN_ALL_ON){
      digitalWrite(relayCtrl1, HIGH); 
      digitalWrite(relayCtrl2, HIGH); 
      digitalWrite(relayCtrl3, HIGH); 
      digitalWrite(relayCtrl4, HIGH); 
      for(int i = 0; i < sizeof(status); i++){
        status[i] = RELAY_IS_ON;
      }
    }

    // This command will put each relay into it's opposite state: relays that
    // are on will be turned off and vice versa. 
    else if(COMMAND == TOGGLE_ALL){
      for(int i = 0; i < sizeof(status); i++){
        if(status[i] == RELAY_IS_ON){
          // Remember that the relays are on pins 0-4, which aligns with the
          // status array. With that, I'll be using the iterator 'i' to write
          // the pins HIGH and LOW. 
          digitalWrite(i, LOW); 
          status[i] = RELAY_IS_OFF;
          continue; 
        }
        //Would be an else statement, but for clarity....
        else if(status[i] == RELAY_IS_OFF){
          digitalWrite(i, HIGH);
          status[i] = RELAY_IS_ON;
        }
      }
    }

  }
}

void requestEvent()
{
  // In the you want to request multiple relay states without having to do
  // multiple "requestFrom"'s in code, the request COMMAND will iterate through
  // the sequential response commands (0x05 -> 0x08). For example, if you
  // request 2 bytes, starting with relay three (0x07), then you'll receive the
  // state of relay three and relay four(0x08). If you request the state of
  // three relays, starting with four, you'll get four(0x08), one(0x05),
  // two(0x06) etc.
  if(COMMAND == RELAY_STATUS_ONE){
      Wire.write(status[0]);
      COMMAND++; 
  }
  if(COMMAND == RELAY_STATUS_TWO){
      Wire.write(status[1]);
      COMMAND++;
  }
  if(COMMAND == RELAY_STATUS_THREE){
      Wire.write(status[2]);
      COMMAND++; 
  } 
  if(COMMAND == RELAY_STATUS_FOUR){
      Wire.write(status[3]);
      COMMAND = 0x05; 
  }
}

void readSystemSettings(void)
{
  //Read what I2C address we should use
  setting_i2c_address = EEPROM.read(LOCATION_I2C_ADDRESS);
  if(setting_i2c_address == 255) {
    setting_i2c_address = I2C_ADDRESS_DEFAULT; //By default, we listen for I2C_ADDRESS_DEFAULT
    EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);
  }
}

//Begin listening on I2C bus as I2C slave using the global variable setting_i2c_address
void startI2C()
{
  Wire.end(); //Before we can change addresses we need to stop
  
  if(digitalRead(addrPin) == HIGH) //Default is HIGH because the pin is not broken out
    Wire.begin(setting_i2c_address); //Start I2C and answer calls using address from EEPROM

  //The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
