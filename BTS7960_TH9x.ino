#include "BTS7960_TH9x.h"

IBusBM ibus;                          // Create iBus Object
BTS7960_TH9x car;

//Function to read the channel value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue)
{
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  
  return constrain(map(ch, 1000, 2000, minLimit, maxLimit), minLimit, maxLimit);
//  if(ch < 1000)
//  {
//    ch = 1000;
//  }
//  if(ch > 2000)
//  {
//    ch = 2000;
//  }
//  map(ch, 1000, 2000, minLimit, maxLimit);
//  return ch;
}

//Function to read the switch value
bool readSwitch(byte channelInput, bool defaultValue)
{
 int intDefaultValue = (defaultValue) ? 100 : 0;
 int ch = readChannel(channelInput, 0, 100, intDefaultValue);
 return (ch > 50);
}

//Transmitter channel values alternate code 
//car.spd = 0;
//long CH2 = 0;
//long CH3 = 0;
//int CH0 = 0;
//bool CH4 = false;                         //Switch mode

//Transmitter channel values
//car.spd = 0;
int CH2 = 0;
int CH3 = 0;
int CH0 = 0;
bool CH4 = false;                         //Switch mode

void setup()
{ 
   Serial.begin(115200);             // Start serial monitor for debugging
   ibus.begin(Serial);               // Attach iBus object to serial port
}

void loop()
{
  // Get RC channel values
  CH0 = readChannel(0, -255, 255, 0);       // Leftshift - Rightshift
  car.spd = readChannel(1, 0, 255, 0);      // Speed (Acceleration)
  CH3 = readChannel(2, -255, 255, 0);       // Forward - Reverse
  CH2 = readChannel(3, -255, 255, 0);       // Left - Right 
  CH4 = readSwitch(4, false);               // CH5 Switch mode

  car.lowSpeedBuzz();

  if(CH2 > 20)
  {
      //spd = (float)(spd*CH2)/255;
      car.front();
      Serial.println("Foreward: ");
      Serial.println(car.spd);
  }
  else if(CH2 < -20)
  {
      //spd = abs((float)(spd*CH2)/255);
      car.back();
      Serial.println("Reverse: ");
      Serial.println(car.spd);
  }
  else if(CH3 > 20)
  {    
      //spd = (float)(spd*CH3)/255;
      car.rightTurn();
      Serial.println("Right Turn: ");
      Serial.println(car.spd);
  }
  else if(CH3 < -20)
  {
      //spd = abs((float)(spd*CH3)/255);
      car.leftTurn();
      Serial.println("Left Turn: ");
      Serial.println(car.spd);
  }
  else if(CH0 > 20 && CH4 == false)
  {
      //spd = abs((float)(spd*CH0)/255);
      car.sharpRightTurnFront();
      Serial.println("Sharp Right Turn Front: ");
      Serial.println(car.spd);
  }
  else if(CH0 < -20 && CH4 == false)
  {
      //spd = abs((float)(spd*CH0)/255);
      car.sharpLeftTurnFront();
      Serial.println("Sharp Left Turn Front: ");
      Serial.println(car.spd);
  }
  else if(CH0 > 20 && CH4 == true)
  {
      //spd = abs((float)(spd*CH0)/255);
      car.sharpRightTurnBack();
      Serial.println("Sharp Right Turn Back: ");
      Serial.println(car.spd);
  }
  else if(CH0 < -20 && CH4 == true)
  {
      //spd = abs((float)(spd*CH0)/255);
      car.sharpLeftTurnBack();
      Serial.print("Sharp Left Turn Back: ");
      Serial.println(car.spd);
  }
  else 
  {
      car.Stp();
      Serial.print("Stop: ");
      Serial.println(car.spd);
  }
}
