#include "FlySkyIBus.h"
#include "BUZZER.h"
#include "BTS7960_Motordriver.h"
#include "LED.h"
#include "CONFIG.h"

//Transmitter channels (previously uint16_t was long for all transmitter variables)
//uint16_t spd = 0;
uint16_t CH0 = 0;
uint16_t CH2 = 0;
uint16_t CH3 = 0;
bool CH4 = false;                   //Switch mode
bool CH6 = false;                   //on off switch

//Transmitter -1 values settings
constexpr uint16_t deadzoneLowerLimit = 1200;
constexpr uint16_t deadzoneUpperLimit = 1800;

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t R_EN1 = 7;  
constexpr uint8_t L_EN1 = 8;
constexpr uint8_t LPWM1 = 6;              //PWM 490hz
constexpr uint8_t RPWM1 = 5;              //PWM 980hz
constexpr uint8_t R_IS1 = -1;        //Alarm pin
constexpr uint8_t L_IS1 = -1;        //Alarm pin

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t R_EN2 = A0;
constexpr uint8_t L_EN2 = A1;
constexpr uint8_t RPWM2 = 9;              //PWM 980hz
constexpr uint8_t LPWM2 = 10;             //PWM 490hz
constexpr uint8_t R_IS2 = -1;        //Alarm pin
constexpr uint8_t L_IS2 = -1;        //Alarm pin

//Led definition section
constexpr uint8_t redLedPin = A4;   //use between 150 ohms to 330 ohms resistor
constexpr uint8_t blueLedPin = 3;

//Buzzer definition section
#define buzzpin 2 //Active buzzer use 100 ohms resistor

/*=====================================================  Object declaration=============================================================*/
FlySkyIBus ibus;                                                      // Create iBus Object
BTS7960 motor1(L_EN1, R_EN1, LPWM1, RPWM1, L_IS1, R_IS1, false);      //Create an object of class motor1
BTS7960 motor2(L_EN2, R_EN2, RPWM2, LPWM2, L_IS1, R_IS2, false);      //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin);                                                //Create object for red led
led blueLed(blueLedPin);                                              //Create object for blue led
buzzer buzz(buzzpin);                                                 //Create object for buzzer
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
   EXTRAOFF = 'x'
};
motorStates motorStatus = motorStates::STOPALL;                        //State variable set to STOP initially
//motorStates motorPrevStatus;                                         //Previous motor state

/*==================================================Function prototyping section========================================================*/
inline void initSystem() __attribute__((always_inline));
inline void standbySystem() __attribute__((always_inline));
/*======================================================================================================================================*/

void initSystem()
{
  //Initlization sequence
  debugln("System initlized, waiting for Transmitter...");
  motor1.enable();  motor2.enable();                                  //Makes all enable pins go high
  blueLed.on();                                                       //Turns the blue led on
  redLed.on();                                                        //Turns the red led on
  buzz.initBuzzer();                                                  //puts the buzzer on
  delay(2);

  //Ready state
  blueLed.off();                                                      //Turns the blue led on
  redLed.on();                                                        //Turns the red led on

  //Initial statemachine setting
  motorStatus = motorStates::STOP;                                    //State variable set to STOP initially
}

void standbySystem()
{
  debugln("Receiver disconnected...");
  motor1.disable(); motor2.disable();
  blueLed.off();  redLed.off();
  buzz.deinitBuzzer();  
}

//namespaces here
