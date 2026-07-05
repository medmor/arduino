#include "car.h"

Car::Car(int in1M1, int in2M1, int in1M2, int in2M2)
{
    motor1.setUp(in2M1, in1M1);
    motor2.setUp(in1M2, in2M2);
}

void Car::forward()
{
    motor1.forward();
    motor2.forward();
}

void Car::backward()
{
    motor1.backward();
    motor2.backward();
}

void Car::left()
{
    motor2.forward();

}

void Car::right()
{
    motor1.forward();

}

void Car::stop()
{
    motor1.stop();
    motor2.stop();
}