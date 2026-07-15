#include <Arduino.h>
#include "motor.h"

class Car
{
public:
    Car(int in1M1, int in2M1, int in1M2, int in2M2/*, int lightPin*/);
    void forward();
    void backward();
    void left();
    void right();
    void stop();
    // void update(unsigned long now);

private:
    Motor motor1, motor2;
    // int lightPin;
    // bool moving = false;
    // unsigned long lastToggle = 0;
    // bool ledState = false;
};