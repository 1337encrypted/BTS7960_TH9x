/*
 * BTS7960.h - Library for controlling a brushed DC motor using the BTS7960 43amp motor driver.
 * Created by Yash Herekar, August 31, 2022.
 * Released into the public domain.
 * Copyright [2022] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

/*Cannot create a .cpp file as  prototypes need the function to be present in the same file*/

#ifndef BTS7960_H
#define BTS7960_H

#include"CONFIG.h"

class BTS7960
{
    private:
    //L_IS1,R_IS1 are not included
    uint8_t R_EN;
    uint8_t L_EN;
    uint8_t R_PWM;
    uint8_t L_PWM;
    //uint8_t R_IS;
    //uint8_t L_IS;
        
    public:
    
    //pwm variable to control the speed of motor
    volatile uint8_t pwm;
    
    /*================================================Function prototyping section========================================================*/
    inline BTS7960() __attribute__((always_inline));
    inline BTS7960(uint8_t, uint8_t) __attribute__((always_inline));
    inline BTS7960(uint8_t, uint8_t, uint8_t, uint8_t) __attribute__((always_inline));
    inline BTS7960(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) __attribute__((always_inline));
    inline void begin() __attribute__((always_inline));
    inline void enable() __attribute__((always_inline));
    inline void disable() __attribute__((always_inline));
    inline void stop() __attribute__((always_inline));
    inline void front() __attribute__((always_inline));
    inline void back() __attribute__((always_inline));
    inline ~BTS7960() __attribute__((always_inline));
    /*====================================================================================================================================*/
};

//Default constructor
BTS7960::BTS7960()
{
    //Motor driver 1 pin definitions.
    this->L_EN = 2;
    this->R_EN = 4;
    this->L_PWM = 5;        //pin 5 has PWM frequency of 980Hz
    this->R_PWM = 6;        //pin 6 has PWM frequency of 980Hz
//    this->L_IS = 7;         //Alarm pin
//    this->R_IS = 8;         //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor with 2 parameters where R_EN and L_EN are hard wired to 3.3v to 5v.
BTS7960::BTS7960(uint8_t L_PWM, uint8_t R_PWM)
{
    //Motor driver 1 pin definitions
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor with 4 parameters, this disables L_IS and R_IS.
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM, uint8_t L_IS, uint8_t R_IS)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz
//    this->L_IS = L_IS;      //Alarm pin
//    this->R_IS = R_IS;      //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
}

void BTS7960::begin()
{
    //Motor driver enable pins set as output and high
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);

    //PWM is for direction and pwm
    pinMode(R_PWM, OUTPUT);
    pinMode(L_PWM, OUTPUT);
}

void BTS7960::enable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
}

void BTS7960::disable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(R_EN, LOW);
    digitalWrite(L_EN, LOW);
}

void BTS7960::stop()
{
    analogWrite(R_PWM,0);
    analogWrite(L_PWM,0);
    
    //R_EN is HIGH then set it low, else dont
    if(digitalRead(R_EN))
    {
        disable();
        debugln("disbale");
    }
}

void BTS7960::front()
{
    //R_EN is LOW then set it high, else dont
    if(!digitalRead(R_EN))
    {
        enable();
        debugln("enable");
    }
    
    analogWrite(L_PWM,0);
    //delayMicroseconds(100);
    analogWrite(R_PWM,pwm);
    //delayMicroseconds(100);
}

void BTS7960::back()
{
    //R_EN is LOW then set it high, else dont
    if(!digitalRead(R_EN))
    {
        enable();
        debugln("enable");
    }
    
    analogWrite(L_PWM,pwm);
    //delayMicroseconds(100);
    analogWrite(R_PWM,0);
    //delayMicroseconds(100);
}

//Destructor
BTS7960::~BTS7960()
{
  debugln("motor object destroyed");
}

#endif  //END BTS7960_H
