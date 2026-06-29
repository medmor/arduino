#include "car.h"

Car::Car(int in1M1, int in2M1, int in1M2, int in2M2)
{
    motor1.setUp(in1M1, in2M1);
    motor2.setUp(in1M2, in2M2);
}

// Motor2 is physically wired backwards, so its forward/backward are swapped.
// Forward/backward are also swapped vs. the originalBLE sketch to match
// the user's motor mounting orientation after the car was sealed.

void Car::forward()
{
    Serial.println("Car::forward()");
    motor1.backward();
    motor2.forward();
}

void Car::backward()
{
    Serial.println("Car::backward()");
    motor1.forward();
    motor2.backward();
}

void Car::left()
{
    Serial.println("Car::left()");
    motor1.forward();
    motor2.forward();
}

void Car::right()
{
    Serial.println("Car::right()");
    motor1.backward();
    motor2.backward();
}

void Car::stop()
{
    Serial.println("Car::stop()");
    motor1.stop();
    motor2.stop();
}