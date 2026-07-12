#include "car.h"

Car::Car(int in1M1, int in2M1, int in1M2, int in2M2, int lightPin)
    : lightPin(lightPin)
{
    motor1.setUp(in2M1, in1M1);
    motor2.setUp(in1M2, in2M2);
    pinMode(lightPin, OUTPUT);
    digitalWrite(lightPin, LOW);
}

void Car::forward()
{
    motor1.forward();
    motor2.forward();
    moving = true;
}

void Car::backward()
{
    motor1.backward();
    motor2.backward();
    moving = true;
}

void Car::left()
{
    motor2.forward();
    moving = true;
}

void Car::right()
{
    motor1.forward();
    moving = true;
}

void Car::stop()
{
    motor1.stop();
    motor2.stop();
    moving = false;
    lastToggle = 0;
}

void Car::update(unsigned long now)
{
    unsigned long interval = moving ? 200 : 1000;
    if (now - lastToggle >= interval)
    {
        ledState = !ledState;
        digitalWrite(lightPin, ledState);
        lastToggle = now;
    }
}