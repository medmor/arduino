#include "car.h"

Car::Car(int in1M1, int in2M1, int in1M2, int in2M2)
{
    motor1.setUp(in1M1, in2M1);
    motor2.setUp(in2M2, in1M2);
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
}

void Car::forward()
{
    Serial.println("Car::forward()");
    motor1.forward();
    motor2.forward();

    accelerating = true;
}

void Car::backward()
{
    motor1.backward();
    motor2.backward();

    accelerating = true;
}

void Car::left()
{
    motor1.forward();

    accelerating = true;
}

void Car::right()
{
    motor2.forward();

    accelerating = true;
}

void Car::stop()
{
    motor1.stop();
    motor2.stop();

    accelerating = false;
    speed = MIN_SPEED;

    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
}

void Car::accelerate()
{
    if (speed < MAX_SPEED && accelerating)
    {
        speed = speed + ACCELERATION;
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
    }
}
