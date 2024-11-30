int TrigPin = 2;
int EchoPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT); // trig as output
  pinMode(EchoPin, INPUT); // echo as input
}

void loop() {
  digitalWrite(TrigPin, HIGH); // sets the pin on
  delayMicroseconds(50); // pause for 50 microseconds
  digitalWrite(TrigPin, LOW);
  int duration = pulseIn(3, HIGH);
  
  int centimeter = duration / 58.2;
  
  Serial.print("duration = ");
  Serial.println(duration);
  Serial.print("centimeter = ");
  Serial.println(centimeter);
  delay(500);
}
