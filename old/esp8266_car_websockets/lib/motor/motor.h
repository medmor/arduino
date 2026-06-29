#include <Arduino.h>

class Motor
{
public:
    void forward();
    void backward();
    void stop();
    void setSpeed(int speed);
    void setUp(int en, int in1, int in2);

private:
    int en, in1, in2, speed = 0;
    bool accelerating = false;
};