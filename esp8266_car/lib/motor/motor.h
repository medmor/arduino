#include <Arduino.h>
#include <Ticker.h>

Ticker accelerator;

int M1_ENA = D7;
int M1_IN1 = D1;
int M1_IN2 = D2;

int M2_ENA = D5;
int M2_IN1 = D3;
int M2_IN2 = D4;

int MIN_SPEED = 150;
int MAX_SPEED = 255;
int ACCELERATION = 5;
float TIME_TO_ACCELERATE = .1;
int speed = MIN_SPEED;

void setupMotores()
{
    pinMode(M1_ENA, OUTPUT);
    pinMode(M2_ENA, OUTPUT);
    pinMode(M1_IN1, OUTPUT);
    pinMode(M1_IN2, OUTPUT);
    pinMode(M2_IN1, OUTPUT);
    pinMode(M2_IN2, OUTPUT);
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, LOW);

    analogWrite(M1_ENA, speed);
    analogWrite(M2_ENA, speed);
}

void accelerate()
{
    // // // Serial.println("Accelerate");
    if (speed < MAX_SPEED)
    {
        speed = speed + ACCELERATION;
        analogWrite(M1_ENA, speed);
        analogWrite(M2_ENA, speed);
    }
}

void motor_forward()
{
    // // Serial.println("Forward");

    digitalWrite(M1_IN1, HIGH);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, HIGH);
    accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_backward()
{
    // // Serial.println("Backward");

    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, HIGH);
    digitalWrite(M2_IN1, HIGH);
    digitalWrite(M2_IN2, LOW);
    accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_left()
{
    // // Serial.println("Left");

    digitalWrite(M1_IN1, HIGH);
    digitalWrite(M1_IN2, LOW);
    accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_right()
{
    // // Serial.println("Right");

    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, HIGH);
    accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_stop()
{
    // // Serial.println("Stop");
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, LOW);
    accelerator.detach();
    speed = MIN_SPEED;
    analogWrite(M1_ENA, speed);
    analogWrite(M2_ENA, speed);
}