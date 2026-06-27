#define AIN1 2
#define AIN2 3
#define BIN1 4
#define BIN2 5

void motorA(int in1, int in2) {
  digitalWrite(AIN1, in1);
  digitalWrite(AIN2, in2);
}
void motorB(int in1, int in2) {
  digitalWrite(BIN1, in1);
  digitalWrite(BIN2, in2);
}
void stop() {
  motorA(LOW, LOW);
  motorB(LOW, LOW);
}
void forward()  { motorA(HIGH, LOW); motorB(HIGH, LOW); }
void backward() { motorA(LOW, HIGH); motorB(LOW, HIGH); }
void left()     { motorA(LOW, HIGH); motorB(HIGH, LOW); }
void right()    { motorA(HIGH, LOW); motorB(LOW, HIGH); }

void setup() {
  Serial.begin(115200);
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  Serial.println("DRV8833 test starting");
}

void loop() {
  Serial.println("Forward 10s"); forward();  delay(10000); stop(); delay(500);
  Serial.println("Backward 10s"); backward(); delay(10000); stop(); delay(500);
  Serial.println("Left 10s");    left();     delay(10000); stop(); delay(500);
  Serial.println("Right 10s");   right();    delay(10000); stop(); delay(2000);
}