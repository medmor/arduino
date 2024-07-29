#include "car.h"

Car::Car(int enM1, int in1M1, int in2M1, int enM2, int in1M2, int in2M2)
{
    this->enM1 = enM1;
    this->in1M1 = in1M1;
    this->in2M1 = in2M1;
    this->enM2 = enM2;
    this->in1M2 = in1M2;
    this->in2M2 = in2M2;

    pinMode(enM1, OUTPUT);
    pinMode(enM2, OUTPUT);
    pinMode(in1M1, OUTPUT);
    pinMode(in2M1, OUTPUT);
    pinMode(in1M2, OUTPUT);
    pinMode(in2M2, OUTPUT);
    digitalWrite(in1M1, LOW);
    digitalWrite(in2M1, LOW);
    digitalWrite(in1M2, LOW);
    digitalWrite(in2M1, LOW);
    analogWrite(enM1, speed);
    analogWrite(enM2, speed);
}

void Car::forward()
{
    digitalWrite(in1M1, HIGH);
    digitalWrite(in2M1, LOW);
    digitalWrite(in1M2, LOW);
    digitalWrite(in2M2, HIGH);

    accelerating = true;
}

void Car::backward()
{
    digitalWrite(in1M1, LOW);
    digitalWrite(in2M1, HIGH);
    digitalWrite(in1M2, HIGH);
    digitalWrite(in2M2, LOW);

    accelerating = true;
}

void Car::left()
{
    digitalWrite(in1M1, HIGH);
    digitalWrite(in2M1, LOW);

    accelerating = true;
}

void Car::right()
{
    digitalWrite(in1M2, LOW);
    digitalWrite(in2M2, HIGH);

    accelerating = true;
}

void Car::stop()
{
    digitalWrite(in1M1, LOW);
    digitalWrite(in2M1, LOW);
    digitalWrite(in1M2, LOW);
    digitalWrite(in2M2, LOW);

    accelerating = false;
    speed = MIN_SPEED;
    analogWrite(enM1, speed);
    analogWrite(enM2, speed);
}

void Car::accelerate()
{
    if (speed < MAX_SPEED && accelerating)
    {
        speed = speed + ACCELERATION;
        analogWrite(enM1, speed);
        analogWrite(enM2, speed);
    }
}
