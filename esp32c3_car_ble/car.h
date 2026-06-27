#include <Arduino.h>
#include "motor.h"

class Car
{
public:
    Car(int in1M1, int in2M1, int in1M2, int in2M2);
    void forward();
    void backward();
    void left();
    void right();
    void stop();

private:
    Motor motor1, motor2;
};