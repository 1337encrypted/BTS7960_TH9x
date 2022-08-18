#include <IBusBM.h>
IBusBM ibus;                          // Create iBus Object

//Motor 1 and 2
#define LPWM1 5
#define RPWM1 6
#define R_EN1 7
#define L_EN1 8

//Motor 3 and 4
#define R_EN2 A0
#define L_EN2 A1
#define LPWM2 10
#define RPWM2 9

//Led and buzzer

#define minSpeed 50
bool buzzState = false;
#define buzpin 2
#define redLed 3
#define blueLed A4

//Transmitter channel values
long spd = 0;
long CH2 = 0;
long CH3 = 0;
long CH0 = 0;
bool CH5 = 0;                         //Switch mode

void setup()
{ 
   Serial.begin(115200);             // Start serial monitor for debugging
   ibus.begin(Serial);               // Attach iBus object to serial port

   //motor 1 and 2
   pinMode(LPWM1, OUTPUT);
   pinMode(RPWM1, OUTPUT);
   pinMode(L_EN1, OUTPUT);
   pinMode(R_EN1, OUTPUT);
   
   //motor 3 and 4
   pinMode(LPWM2, OUTPUT);
   pinMode(RPWM2, OUTPUT);
   pinMode(L_EN2, OUTPUT);
   pinMode(R_EN2, OUTPUT);

   //Buzzer and led
   pinMode(buzpin, OUTPUT);
   pinMode(redLed, OUTPUT);
   pinMode(blueLed, OUTPUT);

   digitalWrite(R_EN1, HIGH);
   digitalWrite(L_EN1, HIGH);
   digitalWrite(R_EN2, HIGH);
   digitalWrite(L_EN2, HIGH);

   Stp();

   //Initilize
   digitalWrite(buzpin, HIGH);
   digitalWrite(redLed, HIGH);
   digitalWrite(blueLed, HIGH);
   delay(2);
   digitalWrite(buzpin, LOW);
   digitalWrite(redLed, HIGH);
   digitalWrite(blueLed, LOW);
}

void loop()
{
  // Get RC channel values
  CH0 = readChannel(0, -255, 255, 0);       // Leftshift - Rightshift
  spd = readChannel(1, 0, 255, 0);          // Speed (Acceleration)
  CH3 = readChannel(2, -255, 255, 0);       // Forward - Reverse
  CH2 = readChannel(3, -255, 255, 0);       // Left - Right 
  CH5 = readSwitch(5, false);               // CH5 Switch mode

  if(spd < minSpeed && buzzState == false)
  {
    digitalWrite(buzpin, HIGH);
    buzzState = true;
  }
  if(spd >= minSpeed && buzzState == true)
  {
    digitalWrite(buzpin, LOW);
    buzzState = false;
  }
  
  if(CH2 > 20)
  {
      //spd = (float)(spd*CH2)/255;
      front();
      Serial.println("Foreward: "+spd);
  }
  else if(CH2 < -20)
  {
      //spd = abs((float)(spd*CH2)/255);
      back();
      Serial.println("Reverse: "+spd);
  }
  else if(CH3 > 20)
  {    
      //spd = (float)(spd*CH3)/255;
      rightTurn();
      Serial.println("Right: "+spd);
  }
  else if(CH3 < -20)
  {
      //spd = abs((float)(spd*CH3)/255);
      leftTurn();
      Serial.println("Left: "+spd);
  }
  else if(CH0 > 20 && CH5 == false)
  {
      //spd = abs((float)(spd*CH0)/255);
      sharpRightTurn();
      Serial.println("RightSharpTurn: "+spd);
  }
  else if(CH0 < -20 && CH5 == false)
  {
      //spd = abs((float)(spd*CH0)/255);
      sharpLeftTurn();
      Serial.println("LeftSharpTurn: "+spd);
  }
  else if(CH0 > 20 && CH5 == true)
  {
      //spd = abs((float)(spd*CH0)/255);
      sharpRightTurnBack();
      Serial.println("RightSharpTurnBack: "+spd);
  }
  else if(CH0 < -20 && CH5 == true)
  {
      //spd = abs((float)(spd*CH0)/255);
      sharpLeftTurnBack();
      Serial.println("LeftSharpTurnBack: "+spd);
  }
  else 
  {
      Stp();
      Serial.println("Stop: ");
  }
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
  //LED
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, LOW);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void back()
{
  //LED 
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,spd);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}

void front()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void leftTurn()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}

void rightTurn()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motor
  analogWrite(RPWM1,spd);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void sharpRightTurn()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,spd);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void sharpLeftTurn()
{
  //LEd
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,spd);
}

void sharpRightTurnBack()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,spd);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}

void sharpLeftTurnBack()
{
  //LED
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  //Motors
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,spd);
  analogWrite(LPWM2,0);
}
