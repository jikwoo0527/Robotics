// 핀 정의
const int motorEnablePin = 5;  // PWM 제어 핀 (속도 제어)
const int motorInput1 = 6;     // 모터 방향 제어 핀 1
const int motorInput2 = 7;     // 모터 방향 제어 핀 2
const int trigPin = 9;         // 초음파 센서 Trig 핀
const int echoPin = 10;        // 초음파 센서 Echo 핀

// 변수 초기화
long duration;
int distance;

// 초기 설정
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  Serial.begin(9600); // 디버깅용 시리얼 모니터 시작
}

// 거리 계산 함수
int calculateDistance() {
  // 초음파 신호 전송
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 초음파 신호 수신 시간 측정
  duration = pulseIn(echoPin, HIGH);

  // 거리 계산 (단위: cm)
  int distance = duration * 0.034 / 2;
  return distance;
}

// 모터 동작 함수: 열림
void openLid() {
  digitalWrite(motorInput1, HIGH); // 모터 방향 설정: 열림
  digitalWrite(motorInput2, LOW);
  analogWrite(motorEnablePin, 150); // 속도 150
  delay(1000); // 열리는 시간 (1초)
  stopMotor();
}

// 모터 동작 함수: 닫힘
void closeLid() {
  digitalWrite(motorInput1, LOW);  // 모터 방향 설정: 닫힘
  digitalWrite(motorInput2, HIGH);
  analogWrite(motorEnablePin, 150); // 속도 150
  delay(1000); // 닫히는 시간 (1초)
  stopMotor();
}

// 모터 정지 함수
void stopMotor() {
  analogWrite(motorEnablePin, 0); // 모터 정지
}

// 메인 루프
void loop() {
  distance = calculateDistance(); // 초음파 센서로 거리 측정
  Serial.print("Distance: ");
  Serial.println(distance);

  // 감지 거리가 20cm 이내일 때 동작
  if (distance <= 20) {
    Serial.println("Object detected! Opening...");
    openLid();    // 바닥 열기
    delay(2000);  // 열린 상태 유지
    Serial.println("Closing...");
    closeLid();   // 바닥 닫기
  }

  delay(100); // 반복 간격
}
