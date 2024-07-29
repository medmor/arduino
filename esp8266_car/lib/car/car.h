#include <Arduino.h>
#include <Ticker.h>

class Car
{
public:
    Car(int enM1, int in1M1, int in2M1, int enM2, int in1M2, int in2M2);
    void forward();
    void backward();
    void left();
    void right();
    void stop();
    void accelerate();

private:
    int enM1, in1M1, in2M1, enM2, in1M2, in2M2,
        MIN_SPEED = 150,
        MAX_SPEED = 255,
        ACCELERATION = .5,
        speed = MIN_SPEED;
    bool accelerating = false;
};
