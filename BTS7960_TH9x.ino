#include <IBusBM.h>
IBusBM ibus;                          // Create iBus Object

int R_EN1 = 2;
int RPWM1 = 3;
int L_EN1 = 4;
int LPWM1 = 5;
int RPWM2 = 6;
int R_EN2 = 7;
int L_EN2 = 8;
int LPWM2 = 9;

long spd = 0;
long CH2 = 0;
long CH3 = 0;
long CH0 = 0;
long CH5 = 0;                         //Switch mode

void setup()
{ 
   Serial.begin(115200);             // Start serial monitor for debugging
   ibus.begin(Serial);               // Attach iBus object to serial port
   
   pinMode(R_EN1, OUTPUT);
   pinMode(R_EN2, OUTPUT);
   pinMode(RPWM1, OUTPUT);
   pinMode(LPWM1, OUTPUT);
   pinMode(RPWM2, OUTPUT);
   pinMode(LPWM2, OUTPUT);
   pinMode(L_EN1, OUTPUT);
   pinMode(L_EN2, OUTPUT);

   digitalWrite(R_EN1, HIGH);
   digitalWrite(L_EN1, HIGH);
   digitalWrite(R_EN2, HIGH);
   digitalWrite(L_EN2, HIGH);

   Stp();
}

void loop()
{
  // Get RC channel values
  CH0 = readChannel(0, -255, 255, 0);       // Leftshift - Rightshift
  spd = readChannel(1, 0, 255, 0);          // Speed (Acceleration)
  CH2 = readChannel(2, -255, 255, 0);       // Forward - Reverse
  CH3 = readChannel(3, -255, 255, 0);       // Left - Right 
  CH5 = readSwitch(5, false);               // CH5 Switch mode

  if(CH2 > 20)
  {
      spd = (float)(spd*CH2)/255;
      front();
      Serial.print("Foreward: ");
  }
  else if(CH2 < -20)
  {
    spd = abs((float)(spd*CH2)/255);
      back();
      Serial.print("Reverse: ");
  }
  else if(CH3 > 20)
  {    
    spd = (float)(spd*CH3)/255;
      rightTurn();
      Serial.print("Right: ");
  }
  else if(CH3 < -20)
  {
      leftTurn();
      spd = abs((float)(spd*CH3)/255);
      Serial.print("Left: ");
  }
  else if(CH0 > 20 && CH5 == 0)
  {
      sharpRightTurn();
      spd = abs((float)(spd*CH0)/255);
      Serial.print("RightSharpTurn: ");
  }
  else if(CH0 < -20 && CH5 == 0)
  {
      sharpLeftTurn();
      spd = abs((float)(spd*CH0)/255);
      Serial.print("LeftSharpTurn: ");
  }
//  else if(CH0 > 100 && CH5)
//  {
//      sharpRightTurnBack();
//      spd = abs((float)(spd*CH0)/255);
//      Serial.print("RightSharpTurnBack: ");
//  }
//  else if(CH0 < -100 && CH5)
//  {
//      sharpLeftTurnBack();
//      spd = abs((float)(spd*CH0)/255);
//      Serial.print("LeftSharpTurnBack: ");
//  }
  else 
  {
      Stp();
      Serial.print("Stop: ");
  }
  Serial.print(spd);
  Serial.println();
}

//Function to read the channel value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue)
{
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return constrain(map(ch, 1000, 2000, minLimit, maxLimit), minLimit, maxLimit);
  //return map(ch, 1000, 2000, minLimit, maxLimit);
}

//Function to read the switch value
bool readSwitch(byte channelInput, bool defaultValue)
{
 int intDefaultValue = (defaultValue) ? 100 : 0;
 int ch = readChannel(channelInput, 0, 100, intDefaultValue);
 return (ch > 50);
}

void Stp()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void back()
{
  analogWrite(RPWM1,spd);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}

void front()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void leftTurn()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}

void rightTurn()
{
  analogWrite(RPWM1,spd);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void sharpRightTurn()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void sharpRightTurnBack()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void sharpLeftTurn()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void sharpLeftTurnBack()
{
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}
