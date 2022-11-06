#include "GLOBALS.h"

void setup()
{ 
  Serial.begin(115200);             // Start serial monitor for debugging
  ibus.begin(Serial);               // Attach iBus object to serial port
  motor1.begin();
  motor2.begin();
  redLed.begin();
  blueLed.begin();
  buzz.begin();

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

  if(CH2 > deadzoneUpperLimit)
  {
      motorStatus = motorStates::FRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH2 < deadzoneLowerLimit)
  {
      motorStatus = motorStates::BACK;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH3 > deadzoneUpperLimit)
  {    
      motorStatus = motorStates::RIGHT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH3 < deadzoneLowerLimit)
  {
      motorStatus = motorStates::LEFT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 > deadzoneUpperLimit && CH4 == false)
  {
      motorStatus = motorStates::SHARPRIGHTFRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 < deadzoneLowerLimit && CH4 == false)
  {
      motorStatus = motorStates::SHARPLEFTFRONT;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 > deadzoneUpperLimit && CH4 == true)
  {
      motorStatus = motorStates::SHARPRIGHTBACK;
      buzzStatus = buzzStates::PASS;
      ledStatus = ledStates::RUN;
  }
  else if(CH0 < deadzoneLowerLimit && CH4 == true)
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

  if(CH6 == true)
  {
    motorStatus = motorStates::STOPALL;
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
    motor1.front(); motor2.stop();
    debug("Sharp Right Front: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::SHARPLEFTFRONT:
    motor1.stop();  motor2.front(); 
    debug("Sharp Left Front: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
   
    case motorStates::SHARPRIGHTBACK:
    motor1.stop();  motor2.back();
    debug("Sharp Right Back: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;
    
    case motorStates::SHARPLEFTBACK:
    motor1.back();  motor2.stop();
    debug("Sharp Left Back: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    break;

    case motorStates::STOP:
    motor1.stop();  motor2.stop();
    debug("Stop: ");  debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
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
