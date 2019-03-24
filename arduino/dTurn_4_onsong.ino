/*****************************************************************************************
 *  dTurn 4 onsong is developed by David C.
 * 
 *  Based on the Adafruit BluefruitLE nRF51 library
 *****************************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"


//pin control
int bt1Pin = A0;
int bt2Pin = A1;
int bt3Pin = A2;
int bt4Pin = A3;
int bt5Pin = A4;
int bt6Pin = A5;
int bt7Pin = A9;
int bt8Pin = A10;

//set keys to send here
char bt1Char = 'a';
char bt2Char = 'z';
char bt3Char = 'e';
char bt4Char = 'r';
char bt5Char = 't';
char bt6Char = 'y';
char bt7Char = 'u';
char bt8Char = 'i';
char bt9Char = 'o';
char bt10Char = 'p';
char bt11Char = 'q';

char keys;
boolean pressKEY = false;
boolean pressCTRL = false;
boolean sendKEY = false;
   
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void)
{

  pinMode(bt1Pin, INPUT_PULLUP);
  pinMode(bt2Pin, INPUT_PULLUP);
  pinMode(bt3Pin, INPUT_PULLUP);
  pinMode(bt4Pin, INPUT_PULLUP);
  pinMode(bt5Pin, INPUT_PULLUP);
  pinMode(bt6Pin, INPUT_PULLUP);
  pinMode(bt7Pin, INPUT_PULLUP);
  pinMode(bt8Pin, INPUT_PULLUP);
  
  //while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit HID Keyboard Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  //Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);
  
  //Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=DTurn for OnSong" )) ) {
    error(F("Could not set device name?"));
  }

  //Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

}

void loop(void)
{

  if(digitalRead(bt1Pin)==LOW){
    delay(100);
      if(digitalRead(bt2Pin)==LOW){
        pressCTRL = true;
        pressKEY = false;
      } else {
        keys = bt1Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt2Pin)==LOW){
    
    delay(100);
      if(digitalRead(bt1Pin)==LOW){
        pressCTRL = true;
        pressKEY = false;
      } else {
        keys = bt2Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt3Pin)==LOW){
      delay(100);
      if(digitalRead(bt4Pin)==LOW){
        keys = bt9Char;
        pressKEY = true;
      } else {
        keys = bt3Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt4Pin)==LOW){
      delay(100);
      if(digitalRead(bt3Pin)==LOW){
        keys = bt9Char;
        pressKEY = true;
      } else {
        keys = bt4Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt5Pin)==LOW){
      delay(100);
      if(digitalRead(bt6Pin)==LOW){
        keys = bt10Char;
        pressKEY = true;
      } else {
        keys = bt5Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt6Pin)==LOW){
      delay(100);
      if(digitalRead(bt5Pin)==LOW){
        keys = bt10Char;
        pressKEY = true;
      } else {
        keys = bt6Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt7Pin)==LOW){
      delay(100);
      if(digitalRead(bt8Pin)==LOW){
        keys = bt11Char;
        pressKEY = true;
      } else {
        keys = bt7Char;
        pressKEY = true;
      }
  } else if(digitalRead(bt8Pin)==LOW){
      delay(100);
      if(digitalRead(bt7Pin)==LOW){
        keys = bt11Char;
        pressKEY = true;
      } else {
        keys = bt8Char;
        pressKEY = true;
      }
  } else {
    pressKEY = false;
    sendKEY = false;
    pressCTRL = false;
  }
  
  if(pressKEY && sendKEY == false){

    ble.print("AT+BleKeyboard=");
    ble.println(keys);

    sendKEY = true;
    delay(100);
  } else if(pressCTRL && sendKEY == false){

    ble.println("AT+BLEHIDCONTROLKEY=0x00B8");
    
    sendKEY = true;
    delay(200);
  }
}
