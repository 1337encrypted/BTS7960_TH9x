#include "GLOBALS.h"

//long spd = 0;
long CH2 = 0;
long CH3 = 0;
long CH0 = 0;
bool CH4 = 0;                         //Switch mode

void setup()
{ 
  Serial.begin(115200);             // Start serial monitor for debugging
  ibus.begin(Serial);               // Attach iBus object to serial port
  motor1.begin();
  motor2.begin();
  redLed.begin();
  blueLed.begin();
  buzz.begin();
}

void loop()
{
  // Get RC channel values
  CH0 = readChannel(0, -255, 255, 0);                           // Leftshift - Rightshift
  motor1.pwm = motor2.pwm = readChannel(1, 0, 255, 0);          // Speed (Acceleration)
  CH3 = readChannel(2, -255, 255, 0);                           // Forward - Reverse
  CH2 = readChannel(3, -255, 255, 0);                           // Left - Right 
  CH4 = readSwitch(4, false);                                   // CH5 Switch mode

  if(CH2 > 20)
  {
      motorStatus = motorStates::FRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH2 < -20)
  {
      motorStatus = motorStates::BACK;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH3 > 20)
  {    
      motorStatus = motorStates::RIGHT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH3 < -20)
  {
      motorStatus = motorStates::LEFT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 > 20 && CH4 == false)
  {
      motorStatus = motorStates::SHARPRIGHTFRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 < -20 && CH4 == false)
  {
      motorStatus = motorStates::SHARPLEFTFRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 > 20 && CH4 == true)
  {
      motorStatus = motorStates::SHARPRIGHTBACK;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 < -20 && CH4 == true)
  {
      motorStatus = motorStates::SHARPLEFTBACK;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else 
  {
      motorStatus = motorStates::STOP;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::STOP;
  }

/*==========================================================ROBOT STATE MACHINE=========================================================*/  
  switch (motorStatus)
  {
    case motorStates::FRONT:
    motor1.front(); motor2.front();
    debug("Front: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::BACK:
    motor1.back();  motor2.back();
    debug("Back: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::LEFT:
    motor1.back();  motor2.front();
    debug("Left: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::RIGHT:
    motor1.front(); motor2.back();
    debug("Right: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
         
    case motorStates::SHARPRIGHTFRONT:
    motor1.front(); motor2.stop();  debug("Right shift: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::SHARPLEFTFRONT:
    motor1.stop();  motor2.front(); 
    debug("Left shift: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
   
    case motorStates::SHARPRIGHTBACK:
    motor1.stop();  motor2.back();
    debug("Right Back: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::SHARPLEFTBACK:
    motor1.back();  motor2.stop();
    debug("Left Back: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;

    case motorStates::STOP:
    motor1.stop();  motor2.stop();
    debug("Stop: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;

//    case motorStates::STOPALL:
//    standbySystem();
//    while(true)
//    {
//      if(Serial.available())
//      {
//        initSystem();
//        break;
//      }
//      else
//        redLed.toggle();
//    }
//    break;
//
//    case motorStates::EXTRAON:                            //Same as EXTRAOFF
//    standbySystem();
//    while(true)
//    {
//      motorStatus = (motorStates)Serial.read();
//      if(motorStates::EXTRAOFF == motorStatus)
//      {
//        initSystem();
//        break;
//      }
//      else if(motorStates::STOPALL == motorStatus)
//      {
//        break;
//      }
//      else
//        redLed.toggle();
//    }
//    break;
//    
//    case motorStates::EXTRAOFF:                           //Same as EXTRAON
//    standbySystem();
//    while(true)
//    {
//      motorStatus = (motorStates)Serial.read();
//      if(motorStates::EXTRAOFF == motorStatus)
//      {
//        initSystem();
//        break;
//      }
//      else if(motorStates::STOPALL == motorStatus)
//      {
//        break;
//      }
//      else
//        redLed.toggle();
//    }
//    break;
//
//    case motorStates::FRONTLIGHTSON:
//    debugln("Front lights on");
//    //Do nothing for now
//    break;
//    case motorStates::FRONTLIGHTSOFF:
//    debugln("Front lights off");
//    //Do nothing for now
//    break;
//    case motorStates::BACKLIGHTSON:
//    debugln("Back lights on");
//    //Do nothing for now
//    break;
//    case motorStates::BACKLIGHTSOFF:
//    debugln("Back lights off");
//    //Do nothing for now
//    break;
//    case motorStates::HORNON:
//    debugln("Horn on");
//    //Do nothing for now
//    break;
//    case motorStates::HORNOFF:
//    debugln("Horn off");
//    //Do nothing for now
//    break;

    default: debugln("Invalid input");
  }

/*==========================================================BUZZER STATE MACHINE========================================================*/
  switch (buzzStatus)
  {
    case buzzStates::ON:
    buzz.on();
    break;

    case buzzStates::OFF:
    buzz.off();
    break;

    case buzzStates::PASS:
    //Do nothing
    break;

    default: debugln("Invalid input");
  }

/*============================================================LED STATE MACHINE=========================================================*/
  switch (ledStatus)
  {
    case ledStates::STOP:
    blueLed.off();
    redLed.on();
    break;
    
    case ledStates::RUN:
    blueLed.on();
    redLed.off();
    break;

    case ledStates::PASS:
    //Do nothing
    break;

    default: debugln("Invalid input");
  }
}
