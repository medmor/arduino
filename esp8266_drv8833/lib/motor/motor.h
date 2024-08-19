#include <Arduino.h>

class Motor
{
public:
    void forward();
    void backward();
    void stop();
    void setSpeed(int speed);
    void setUp(int in1, int in2);

private:
    int in1, in2, speed = 255;
    bool accelerating = false;
};