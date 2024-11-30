#include <Servo.h>
Servo left_motor;
Servo right_motor;

int TrigPin = 2;
int EchoPin = 3;

int count = 0; // 금속 탐지 기준을 충족한 횟수
int motor_activate = 0; // 모터 작동 횟수
float THRESHOLD = 610.0; // 금속 탐지 기준

int left_motor_angle = 120; // 설정한 임시 각도
int right_motor_angle = 120; // 설정한 임시 각도


  
void setup() {
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT); // trig as output
  pinMode(EchoPin, INPUT); // echo as input
}


void loop() {
  // Hypersonic sensor
  digitalWrite(TrigPin, HIGH); // 초음파를 쏘고 받는 부분
  delayMicroseconds(50);
  digitalWrite(TrigPin, LOW);
  int duration = pulseIn(3, HIGH);
  int centimeter = duration / 58.2;
 
  Serial.print("centimeter = "); // 측정한 거리 출력
  Serial.println(centimeter);

  // SG90 motor
  left_motor.attach(9); // left_motor에 핀번호 9번 할당
  right_motor.attach(10); // right_motor에 핀번호 10번 할당
  left_motor.write(0); // left_motor 초기화
  right_motor.write(0); // right_motor 초기화
  
  if (centimeter > 0 && centimeter < 10) { // 쓰레기 감지
     int is_metal = 0; // 금속 탐지 여부
     
     for (int i=0;i<20;i++) {
      // Metal sensor
      float val = analogRead(0) * 5.0;
      Serial.println(val); // 금속 탐지기가 읽은 값 출력
      if (motor_activate == 0) { // 모터가 작동 한 적이 없을 경우
        if (val == THRESHOLD) { // 임계값 처음 인식
          count = 1;
        } else {
          count = 0;
        }

        if (count == 1) { // 임계값 두 번째 인식
          if (val == THRESHOLD) {
            count = 2;
          }
        } else {
          count = 0;
        }
    
        if (count == 2) { // 임계값 세 번째 인식
          if (val == THRESHOLD) {
            is_metal = 1; // 금속으로 판단
            left_motor.write(left_motor_angle); // 금속으로 떨어뜨리는 왼쪽 motor 작동
            
            Serial.println("Left Motor Activate");
            count = 0; // 모터를 작동했으므로 금속 탐지 횟수 초기화
            motor_activate = 1; // 모터가 작동했으므로 1로 변경
          }
        }
      } else { // 모터가 작동한 적이 있을 경우
        delay(3000); // 금속으로 떨어뜨리는 왼쪽 motor가 작동한 후 물건이 떨어질 때까지 기다리는 시간
        left_motor.write(0); // Motor 제자리
        Serial.println("Motor deactivated");
        motor_activate = 0; // 금속 떨어뜨리는 과정이 끝났으므로 모터 작동 횟수를 0으로 초기화
        break; // for문 탈출
      }
      delay(200); // 금속 센서 값을 한 번 읽어오고 기다리는 시간
     }
     
     if (is_metal == 0) { // 비금속으로 판단했을 경우
      right_motor.write(right_motor_angle); // 비금속으로 떨어뜨리는 오른쪽 motor 작동
      delay(3000); // 비금속으로 떨어뜨리는 오른쪽 motor가 작동한 후 물건이 떨어질 때까지 기다리는 시간
      right_motor.write(0); // motor 제자리
      Serial.println("Right Motor Activate");
     }
  delay(100); // 초음파 값을 한 번 읽어오고 기다리는 시간
  }
}
