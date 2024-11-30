 void setup() {
  Serial.begin(9600);
}

void loop() {
  float val = analogRead(0)*5.;
  Serial.println(val);

  delay(100);
}
