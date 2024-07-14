#include <Ticker.h>
#include "log.h"

Ticker accelerator;

// For now my motor driver is malfonctioning. only output A is working.
// So I'm commenting out the other pins.
// Support only forward and backward movements.
// I will fix it later.

int M1_ENA = D7;
int M1_IN1 = D1;
int M1_IN2 = D2;
// int M2_ENA = D5;
// int M2_IN1 = D3;
// int M2_IN2 = D4;

int MIN_SPEED = 200;
int MAX_SPEED = 255;
int speed = MIN_SPEED;

void setupMotores()
{
    pinMode(M1_ENA, OUTPUT);
    //   pinMode(M2_ENA, OUTPUT);
    pinMode(M1_IN1, OUTPUT);
    pinMode(M1_IN2, OUTPUT);
    //   pinMode(M2_IN1, OUTPUT);
    //   pinMode(M2_IN2, OUTPUT);
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    //   digitalWrite(M2_IN1, LOW);
    //   digitalWrite(M2_IN2, LOW);

    analogWrite(M1_ENA, speed);
    //   analogWrite(M2_ENA, 255);
}

void accelerate()
{
    log("Accelerate");
    if (speed < MAX_SPEED)
    {
        speed = speed + 10;
        analogWrite(M1_ENA, speed);
        // analogWrite(M2_ENA, 255);
    }
}

void motor_forward()
{
    log("Forward");

    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, HIGH);
    accelerator.attach(100, accelerate);
    // digitalWrite(M2_IN1, LOW);
    // digitalWrite(M2_IN2, HIGH);
}

void motor_backward()
{
    log("Backward");

    digitalWrite(M1_IN1, HIGH);
    digitalWrite(M1_IN2, LOW);
    accelerator.attach(100, accelerate);
    // digitalWrite(M2_IN1, HIGH);
    // digitalWrite(M2_IN2, LOW);
}

// void motor_left(){
//     log("Left");

//     digitalWrite(M1_IN1, HIGH);
//     digitalWrite(M1_IN2, LOW);
//     digitalWrite(M2_IN1, LOW);
//     digitalWrite(M2_IN2, HIGH);
// }

// void motor_right(){
//     log("Right");

//     digitalWrite(M1_IN1, LOW);
//     digitalWrite(M1_IN2, HIGH);
//     digitalWrite(M2_IN1, HIGH);
//     digitalWrite(M2_IN2, LOW);
// }

void motor_stop()
{
    log("Stop");
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    accelerator.detach();
    speed = MIN_SPEED;
    analogWrite(M1_ENA, speed);
    // digitalWrite(M2_IN1, LOW);
    // digitalWrite(M2_IN2, LOW);
}
