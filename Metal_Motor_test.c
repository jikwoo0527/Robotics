#include <Servo.h>
Servo left_motor;
Servo right_motor;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int THRESHOLD = 5; // 설정한 임시 기준
  int angle = 50; // 설정한 임시 각도

  float measure = analogRead(A0) // 아날로그 입력 신호 받음
  left_motor.attach(9); // left_motor에 핀번호 9번 할당
  right_motor.attach(10); // right_motor에 핀번호 10번 할당
  
  float metal_val = measure * 5.0 / 1024.0; // 아날로그 입력 신호를 실제 전압으로 변환
  Serial.println(metal_val); // 실제 전압 값 출력

  if(metal_val > THRESHOLD) { // 금속일 경우
    left_motor.write(angle);
    left_motor.write(0);
    Serial.println("Left Motor Activate");
  } 
  else { // 비금속일 경우
    right_motor.write(angle);
    right_motor.write(0);
    Serial.println("Right Motor Activate");
  }

  Serial.println();
  delay(100);
}
