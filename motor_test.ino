#include <Servo.h>
Servo left_motor;
Servo right_motor;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int angle = 120; // 설정한 임시 각도

  left_motor.attach(9); // left_motor에 핀번호 9번 할당
  right_motor.attach(10); // right_motor에 핀번호 10번 할당
  
  left_motor.write(angle);
  delay(500);
  left_motor.write(0);
  Serial.println("Left Motor Activate");
   
  right_motor.write(angle);
  delay(500);
  right_motor.write(0);
  Serial.println("Right Motor Activate");

  Serial.println();
  delay(100);
}
