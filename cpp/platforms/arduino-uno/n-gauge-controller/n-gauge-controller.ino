const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
}

// モーターを前進させる（正転）
void forward(int pwm) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);  // pwm値：0〜255
}

// モーターを後退させる（反転）
void reverse(int pwm) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);
}

// モーターを停止
void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void go(int max) {
  int i;
  for(i=0;i<max;i++) {
    analogWrite(ENA, i);
    delay(20);
  }
  
  //reverse(120);
  for(i=max;i>=0;i--) {
    analogWrite(ENA, i);
    delay(20);
  }

}

void loop() {
  forward(0);
  go(120);
  delay(2000);
  reverse(0);
  go(120);
  delay(2000);
  stopMotor();
}
