#include <Arduino.h>
#include <motor.h>
class Car
{
public:
    Car(int enM1, int in1M1, int in2M1, int enM2, int in1M2, int in2M2);
    void forward();
    void backward();
    void left();
    void right();
    void stop();

private:
    Motor motor1, motor2;
    int MIN_SPEED = 150,
        MAX_SPEED = 255,
        ACCELERATION = .5,
        speed = MIN_SPEED;
    bool accelerating = false;
    void accelerate();
};
