#include <motor.h>

void Motor::setUp(int in1, int in2)
{
    this->in1 = in1;
    this->in2 = in2;

    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    stop();
}

void Motor::forward()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}

void Motor::backward()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}

void Motor::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

void Motor::setSpeed(int speed)
{
    this->speed = speed;
}
