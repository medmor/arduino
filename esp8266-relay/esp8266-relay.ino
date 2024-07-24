
const int relay = D1;

void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
}

void loop()
{

  digitalWrite(relay, LOW);
  Serial.println("Current Flowing");
  delay(5000);

  digitalWrite(relay, HIGH);
  Serial.println("Current not Flowing");
  delay(5000);
}