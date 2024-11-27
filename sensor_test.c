void setup() {
  Serial.begin(9600);
}

void loop() {
  int THRESHOLD = 5;
  float measure = analogRead(0) // 아날로그 입력 신호 받음

  float metal_val = measure * 5.0 / 1024.0; // 아날로그 입력 신호를 실제 전압으로 변환
  Serial.println(metal_val); // 실제 전압 값 출력

  if(metal_val > THRESHOLD) {
    Serial.println("Right Motor Activate");
  } 
  else {
    Serial.println("Left Motor Deactivate");
  }

  Serial.println();
  delay(100);
}
