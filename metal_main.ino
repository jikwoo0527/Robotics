#include <Servo.h>
Servo left_motor;
Servo right_motor;

int TrigPin = 2;
int EchoPin = 3;

int detect_count = 0; // 금속 탐지 기준을 충족한 횟수
int motor_activate = 0; // 모터 작동 횟수
int initial_activate = 0; // THRESHOLD 설정 여부
float initial_total = 0; // 금속 탐지 값 initial_count번의 합
int initial_count = 0; // THRESHOLD 초기화할 때 700 미만의 값이 인식된 횟수
float THRESHOLD; // 금속 탐지 기준

int left_motor_angle = 120; // 설정한 임시 각도
int right_motor_angle = 120; // 설정한 임시 각도


  
void setup() {
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT); // trig as output
  pinMode(EchoPin, INPUT); // echo as input
}



void loop() {
  // Metal sensor 초기값 설정
  if (initial_activate == 0) {
    for (int i=0;i<30;i++) {
      float initial_val = analogRead(0) * 5.0;
      if (initial_val < 700.0) {
        Serial.print("Initial value: ");
        Serial.println(initial_val); // 금속 탐지기가 읽은 값 출력
        initial_total = initial_total + initial_val;
        initial_count++;
      }
      delay(100);
    }
    THRESHOLD = initial_total / initial_count; // 금속 탐지 기준 설정
    Serial.print("Initial total: ");
    Serial.println(initial_total); // 금속 탐지기가 읽은 값 출력
    Serial.print("THRESHOLD: ");
    Serial.println(THRESHOLD);
    initial_activate = 1; // THRESHOLD 초기화 된 상태로 변경
  }
  
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
  
  if (centimeter > 0 && centimeter < 8) { // 쓰레기 감지
     int is_metal = 0; // 금속 탐지 여부
     
     for (int i=0;i<20;i++) {
      // Metal sensor
      float val = analogRead(0) * 5.0;
      Serial.println(val); // 금속 탐지기가 읽은 값 출력
      if (motor_activate == 0) { // 금속으로 떨어뜨리는 모터가 작동 한 적이 없을 경우
        if (val < THRESHOLD - 4.5) { // 임계값 처음 인식
          detect_count = 1;
        } else {
          detect_count = 0;
        }

        if (detect_count == 1) { // 임계값 두 번째 인식
          if (val < THRESHOLD - 4.5) {
            detect_count = 2;
          }
        } else {
          detect_count = 0;
        }
    
        if (detect_count == 2) { // 임계값 세 번째 인식
          if (val < THRESHOLD - 4.5) {
            is_metal = 1; // 금속으로 판단
            right_motor.write(right_motor_angle); // 금속으로 떨어뜨리는 오른쪽 motor 작동
            
            Serial.println("Left Motor Activated");
            detect_count = 0; // 모터를 작동했으므로 금속 탐지 횟수 초기화
            motor_activate = 1; // 모터가 작동했으므로 1로 변경
          }
        }
      } else { // 금속으로 떨어뜨리는 모터가 작동한 적이 있을 경우
        delay(3000); // 금속으로 떨어뜨리는 왼쪽 motor가 작동한 후 물건이 떨어질 때까지 기다리는 시간
        right_motor.write(0); // Motor 제자리
        Serial.println("Left Motor Deactivated");
        motor_activate = 0; // 금속 떨어뜨리는 과정이 끝났으므로 모터 작동 횟수를 0으로 초기화
        initial_activate = 0; // 한 과정이 끝났으므로 다시 초기값을 설정하도록 함
        break; // for문 탈출
      }
      delay(200); // 금속 센서 값을 한 번 읽어오고 기다리는 시간
     }
     
     if (is_metal == 0) { // 비금속으로 판단했을 경우
      left_motor.write(right_motor_angle); // 비금속으로 떨어뜨리는 왼쪽 motor 작동
      Serial.println("Right Motor Activated");
      delay(3000); // 비금속으로 떨어뜨리는 오른쪽 motor가 작동한 후 물건이 떨어질 때까지 기다리는 시간
      left_motor.write(0); // motor 제자리
      Serial.println("Right Motor Deactivated");
      initial_activate = 0; // 한 과정이 끝났으므로 다시 초기값을 설정하도록 함
     }
  delay(100); // 초음파 값을 한 번 읽어오고 기다리는 시간
  }
}
