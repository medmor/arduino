#include "log.h"

int M1_ENA = D7;
int M1_IN1 = D1;
int M1_IN2 = D2;
int M2_ENA = D5;
int M2_IN1 = D3;
int M2_IN2 = D4;

void setupMotores() {
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

  analogWrite(M1_ENA, 255);
  analogWrite(M2_ENA, 255);
}


void motor_forward(){
    log("Forward");

    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, HIGH);
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, HIGH);
}

void motor_backward(){
    log("Backward");

    digitalWrite(M1_IN1, HIGH);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, HIGH);
    digitalWrite(M2_IN2, LOW);
}

void motor_left(){
    log("Left");

    digitalWrite(M1_IN1, HIGH);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, HIGH);
}

void motor_right(){
    log("Right");

    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, HIGH);
    digitalWrite(M2_IN1, HIGH);
    digitalWrite(M2_IN2, LOW);
}

void motor_stop(){
    log("Stop");
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, LOW);
}