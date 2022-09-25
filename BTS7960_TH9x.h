/*
 * Motor 1 = R_EN1, L_EN1, RPWM1, LPWM1;
 * Motor 2 = R_EN2, L_EN2, RPWM2, LPWM2;
 */

#ifndef BTS7960_TH9x_H
#define BTS7960_TH9x_H

#include <IBusBM.h>
#include <Arduino.h>

class BTS7960_TH9x
{
  private:

    //Led and buzzer for feedback
    #define redLed 3
    #define blueLed A4
    #define buzzPin 2
    #define minSpeed 30               //set the minimum speed for which buzzer should activate
    bool buzzState = false;           //mutex variable for setting the lock
    
    //Motor 1 and 2
    #define R_EN1 7
    #define L_EN1 8
    #define RPWM1 6
    #define LPWM1 5
//  #define R_IS1   Alarm pin
//  #define L_IS1   Alarm pin
    
    //Motor 3 and 4
    #define R_EN2 A0
    #define L_EN2 A1
    #define RPWM2 9
    #define LPWM2 10
//  #define R_IS2   Alarm pin
//  #define L_IS2   Alarm pin

  public:

  volatile uint8_t spd;                 //volatile as it shouldn't be ignored by the compiler, speed variable to control the speed

  inline BTS7960_TH9x() __attribute__((always_inline));
  inline void __initSystem() __attribute__((always_inline));    //__ internal function to the class
  inline void lowSpeedBuzz() __attribute__((always_inline));
  inline void Stp() __attribute__((always_inline));
  inline void front() __attribute__((always_inline));
  inline void back() __attribute__((always_inline));
  inline void leftTurn() __attribute__((always_inline));
  inline void rightTurn() __attribute__((always_inline));
  inline void sharpLeftTurnFront() __attribute__((always_inline));
  inline void sharpRightTurnFront() __attribute__((always_inline));
  inline void sharpRightTurnBack() __attribute__((always_inline));
  inline void sharpLeftTurnBack() __attribute__((always_inline));
//  inline void leftShift() __attribute__((always_inline));
//  inline void rightShift() __attribute__((always_inline));
};    

BTS7960_TH9x::BTS7960_TH9x()
{
  // Led pins set as output
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  
  //Motor driver enable pins set as output
  pinMode(R_EN1, OUTPUT);
  pinMode(L_EN1, OUTPUT);
  pinMode(R_EN2, OUTPUT);
  pinMode(L_EN2, OUTPUT);
  
  //PWM is for direction and speed
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);
   
  __initSystem();
  
  this->buzzState = false;
  this->spd = 0;
}

void BTS7960_TH9x::lowSpeedBuzz()
{
  if(spd < minSpeed && buzzState == false)
  {
    digitalWrite(buzzPin, HIGH);
    buzzState = true;
  }
  else if(spd >= minSpeed && buzzState == true)
  {
    digitalWrite(buzzPin, LOW);
    buzzState = false;
  }
}

void BTS7960_TH9x::__initSystem()
{
  //Initilize the Led and Buzzer
  Serial.println("Initilizing ...");
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(buzzPin, HIGH);
  delay(2);
  digitalWrite(blueLed, LOW);
  digitalWrite(buzzPin, LOW);
  
  //Set all enable pins to high to activate the motor driver
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH); 
  Serial.println("Done initilizing");
}

void BTS7960_TH9x::Stp()
{
  //LED
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, LOW);
  //MOTORS
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0);
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0);
}


void BTS7960_TH9x::back()
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

void BTS7960_TH9x::front()
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

void BTS7960_TH9x::leftTurn()
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

void BTS7960_TH9x::rightTurn()
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

void BTS7960_TH9x::sharpRightTurnFront()
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

void BTS7960_TH9x::sharpLeftTurnFront()
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

void BTS7960_TH9x::sharpRightTurnBack()
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

void BTS7960_TH9x::sharpLeftTurnBack()
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

#endif
