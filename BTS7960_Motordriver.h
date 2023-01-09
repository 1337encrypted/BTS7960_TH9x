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
    uint8_t L_EN;
    uint8_t R_EN;
    uint8_t L_PWM;
    uint8_t R_PWM;
    //uint8_t L_IS;
    //uint8_t R_IS;
        
    public:
    //pwm variable to control the speed of motor
    volatile uint8_t pwm;
    
    /*=============================================Function prototyping section=====================================================*/
    //inline BTS7960() __attribute__((always_inline));
    //inline BTS7960(uint8_t, uint8_t) __attribute__((always_inline));
    inline BTS7960(uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1) __attribute__((always_inline));
    //inline BTS7960(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) __attribute__((always_inline));
    inline void begin() __attribute__((always_inline));
    inline void enable() __attribute__((always_inline));
    inline void disable() __attribute__((always_inline));
    inline void stop() __attribute__((always_inline));
    inline void front() __attribute__((always_inline));
    inline void back() __attribute__((always_inline));
    inline ~BTS7960() __attribute__((always_inline));
    /*===============================================================================================================================*/
};

//Parametrised constructor with 4 parameters, this disables L_IS and R_IS.
BTS7960::BTS7960(uint8_t L_PWM, uint8_t R_PWM, uint8_t L_EN, uint8_t R_EN)
{
    //Motor driver 1 pin definitions
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz
    this->L_EN = L_EN;
    this->R_EN = R_EN;

    //Set the global pwm variable to 255
    this->pwm = 255;

    //Begin and enable happens after object construction
    begin();
}

void BTS7960::begin()
{
    
    //PWM is for direction and pwm
    if(this->L_PWM>0 && this->R_PWM>0)
    {
        pinMode(this->L_PWM, OUTPUT);
        pinMode(this->R_PWM, OUTPUT);
    }
    else
    {
        debugln("L_PWM and R_PWM are not set as output");
    }
    
    //Motor driver enable pins set as output and high
    if(this->L_EN>0 && this->R_EN>0)
    {
        pinMode(this->L_EN, OUTPUT);
        pinMode(this->R_EN, OUTPUT);
        enable();
    }
    else
    {
        debugln("L_EN and R_EN pins not set as output");
    }
}

void BTS7960::enable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(this->L_EN, HIGH);
    digitalWrite(this->R_EN, HIGH);

}

void BTS7960::disable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(L_EN, LOW);
    digitalWrite(R_EN, LOW);
}

void BTS7960::stop()
{
    analogWrite(this->L_PWM,0);
    analogWrite(this->R_PWM,0);
    
    //R_EN is HIGH then set it low, else dont
    if(digitalRead(this->L_EN))
    {
        disable();
        debugln("disbale");
    }
}

void BTS7960::front()
{
    //R_EN is LOW then set it high, else dont
    if(!digitalRead(this->L_EN))
    {
        enable();
        debugln("enable");
    }
    
    analogWrite(this->L_PWM,0);
    //delayMicroseconds(100);
    analogWrite(this->R_PWM,pwm);
    //delayMicroseconds(100);
}

void BTS7960::back()
{
    //R_EN is LOW then set it high, else dont
    if(!digitalRead(this->L_EN))
    {
        enable();
        debugln("enable");
    }
    
    analogWrite(this->L_PWM,pwm);
    //delayMicroseconds(100);
    analogWrite(this->R_PWM,0);
    //delayMicroseconds(100);
}

//Destructor
BTS7960::~BTS7960()
{
  debugln("motor object destroyed");
}

#endif  //END BTS7960_H
