#include "FlySkyIBus.h"
#include <BUZZER.h>
#include <BTS7960_Motordriver.h>
#include <LED.h>

/* 
 * Debug on and off enables code optimization
 * DEBUG 0 will ignore all Serial.print functions
 * DEBUG 1 will will add back Serial.print functions back to the code
 */
 
#define DEBUG 1

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x) 
#endif

//Led definition section
constexpr uint8_t redLedPin = A4;  //use between 150 ohms to 330 ohms resistor
constexpr uint8_t blueLedPin = 3;

//Buzzer definition section
#define buzzpin 2 //Active buzzer use 100 ohms resistor

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t R_EN1 = 7;  
constexpr uint8_t L_EN1 = 8;
constexpr uint8_t RPWM1 = 6;    //PWM 490hz
constexpr uint8_t LPWM1 = 5;    //PWM 980hz
//constexpr uint8_t R_IS1       //Alarm pin
//constexpr uint8_t L_IS1       //Alarm pin

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t R_EN2 = A0;
constexpr uint8_t L_EN2 = A1;
constexpr uint8_t RPWM2 = 9;       //PWM 980hz
constexpr uint8_t LPWM2 = 10;      //PWM 490hz
//constexpr uint8_t R_IS2        //Alarm pin
//constexpr uint8_t L_IS2        //Alarm pin                                                     //Key for the switch case

/*=====================================================  Object declaration=============================================================*/
BTS7960 motor1(L_EN1, R_EN1, LPWM1, RPWM1);                           //Create an object of class motor1
BTS7960 motor2(L_EN2, R_EN2, RPWM2, LPWM2);                           //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin);                                                //Create object for red led
led blueLed(blueLedPin);                                              //Create object for blue led
buzzer buzz(buzzpin);                                                 //Create object for buzzer
FlySkyIBus ibus;                                                      // Create iBus Object
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++MOTOR STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

enum class motorStates : uint8_t
{
   FRONT = 'F',
   BACK = 'B',
   LEFT = 'L',
   RIGHT = 'R',
   SHARPLEFTFRONT = 'G',
   SHARPRIGHTFRONT = 'I',
   SHARPLEFTBACK = 'H',
   SHARPRIGHTBACK = 'J',
   STOP = 'S',
   STOPALL = 'D',
   FRONTLIGHTSON = 'W',
   FRONTLIGHTSOFF = 'w',
   BACKLIGHTSON = 'U',
   BACKLIGHTSOFF = 'u',
   HORNON = 'V',
   HORNOFF = 'v',
   EXTRAON = 'X',
   EXTRAOFF = 'x',
};
motorStates motorStatus = motorStates::STOPALL;                   //State variable set to STOP initially
//motorStates motorPrevStatus;                                    //Previous motor state

enum class buzzStates : uint8_t
{
  OFF,
  ON,
  PASS
};
buzzStates buzzStatus = buzzStates::PASS;                         //Buzzer state initilally is set to pass

enum class ledStates : uint8_t
{
  STOP,
  RUN,
  PASS
};
ledStates ledStatus = ledStates::PASS;                            //Led state initilally is set to pass

/*==================================================Function prototyping section========================================================*/
inline void initSystem() __attribute__((always_inline));
inline void standbySystem() __attribute__((always_inline));
/*======================================================================================================================================*/

void initSystem()
{
  debugln("System initlized, waiting for bluetooth connection...");
  motor1.enable();  motor2.enable();                               //Makes all enable pins go high
  blueLed.on();                                                    //Turns the blue led on
  redLed.on();                                                     //Turns the red led on
  buzz.initBuzzer();                                               //puts the buzzer on
  delay(2);
  blueLed.off();                                                   //Turns the blue led on
  redLed.on();                                                     //Turns the red led on
  motorStatus = motorStates::STOP;                                 //State variable set to STOP initially
  buzzStates buzzStatus = buzzStates::PASS;                        //Buzzer state initilally is set to pass
  ledStates ledStatus = ledStates::STOP;                           //Led state initilally is set to pass 
}

void standbySystem()
{
  debugln("Bluetooth disconnected...");
  blueLed.off();  redLed.off();
  motor1.disable(); motor2.disable();
  buzz.deinitBuzzer();  
}

//namespaces here
