#include "GLOBALS.h"

void setup()
{ 
  Serial.begin(115200);             // Start serial monitor for debugging
  ibus.begin(Serial);               // Attach iBus object to serial port
  initSystem();
}

void loop()
{
  ibus.loop();

  
  // Flysky Th9x
  CH0 = ibus.readChannel(0);                                                  // Leftshift - Rightshift
  motor1.pwm = motor2.pwm = int((ibus.readChannel(1)-1000)*0.255);            // Speed (Acceleration)
  CH2 = ibus.readChannel(2);                                                  // Forward - Reverse
  CH3 = ibus.readChannel(3);                                                  // Left - Right 
  CH4 = ibus.readSwitch(4);                                                   // CH4 Switch mode
  //CH5 is same as CH0
  CH6 = ibus.readSwitch(6);                                                   // CH6 on off switch

//  // Radiomaster TXI6S
//  CH0 = ibus.readChannel(0);                                                // Leftshift - Rightshift
//  motor1.pwm = motor2.pwm = int((ibus.readChannel(2)-1000)*0.255);          // Speed (Acceleration)
//  CH2 = ibus.readChannel(1);                                                // Forward - Reverse
//  CH3 = ibus.readChannel(3);                                                // Left - Right 
//  CH4 = ibus.readSwitch(4);                                                 // CH4 Switch mode
//  CH5 = ibus.readSwitch(5);                                                 // CH6 on off switch (Check it once)

  if(CH2 > 1800)
  {
      motorStatus = motorStates::FRONT;
  }
  else if(CH2 < 1200 && CH2 > 0)
  {
      motorStatus = motorStates::BACK;
  }
  else if(CH3 > 1800)
  {    
      motorStatus = motorStates::RIGHT;
  }
  else if(CH3 < 1200 && CH3 > 0)
  {
      motorStatus = motorStates::LEFT;
  }
  else if(CH0 > 1800 && CH4 == false)
  {
      motorStatus = motorStates::SHARPRIGHTFRONT;
  }
  else if(CH0 < 1200 && CH0 > 0 && CH4 == false)
  {
      motorStatus = motorStates::SHARPLEFTFRONT;
  }
  else if(CH0 > 1800 && CH4 == true)
  {
      motorStatus = motorStates::SHARPRIGHTBACK;
  }
  else if(CH0 < 1200 && CH0 > 0 && CH4 == true)
  {
      motorStatus = motorStates::SHARPLEFTBACK;
  }
  else 
  {
      motorStatus = motorStates::STOP;
  }

  if(CH6 == true)
  {
    motorStatus = motorStates::STOPALL;
  }
  
/*==========================================================ROBOT STATE MACHINE=========================================================*/  
  switch (motorStatus)
  {
    case motorStates::FRONT:
    debug("Front: ");
    motor1.front(); 
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::BACK:
    debug("Back: ");
    motor1.back();  
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::LEFT:
    debug("Left: ");
    motor1.back();
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::RIGHT:
    debug("Right: ");
    motor1.front();
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
         
    case motorStates::SHARPRIGHTFRONT:
    debug("Sharp Right Front: ");
    motor1.front();
    debug(" : ");
    motor2.stop();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::SHARPLEFTFRONT:
    debug("Sharp Left Front: ");
    motor1.stop();
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off(); 
    break;
   
    case motorStates::SHARPRIGHTBACK:
    debug("Sharp Right Back: ");
    motor1.stop();
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::SHARPLEFTBACK:
    debug("Sharp Left Back: ");
    motor1.back();
    debug(" : ");
    motor2.stop();
    blueLed.on();
    redLed.off();
    break;

    case motorStates::STOP:
    debug("Stop: ");
    motor1.stop();
    debug(" : ");
    motor2.stop();
    blueLed.off();
    redLed.on();
    break;

    case motorStates::STOPALL:
    standbySystem();
    while(true)
    {
      ibus.loop();
      CH6 = ibus.readSwitch(6);                                                 // CH6 on off switch
      if(CH6 == false)
      {
        initSystem();
        break;
      }
      else
        redLed.toggle();
    }
    break;

//    case motorStates::EXTRAON:                        
//    debugln("Extra on");
//    break;
//    case motorStates::EXTRAOFF:                          
//    debugln("Extra off");
//    break;
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
  debugln();
}
