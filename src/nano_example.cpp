#include <Arduino.h>

// D5ピンにLEDを接続（抵抗を必ず入れてください）

#if 0

const int interruptA = 0;   // Interrupt 0 is pin 2
const int interruptB = 45;  // Interrupt 45 is pin 3
int CLK = 2;                // PIN2
int DAT = 3;                // PIN3
int BUTTON = 4;             // PIN4
int LED1 = 5;               // PIN5
int LED2 = 6;               // PIN6
volatile int COUNT = 0;

volatile bool flag1 = false;
volatile bool flag2 = false;

void test() {
  int clk = digitalRead(CLK);
  int dat = digitalRead(DAT);
  
  if (clk == dat) {
    COUNT++;
  } else {
    COUNT--;
  }
  if (COUNT > 3) {
    COUNT = 0;
  }
  if (COUNT < 0) {
    COUNT = 3;
  }
  Serial.println("clk:"+String(clk) + ", dat:"+String(dat) + " " + String(COUNT));
}

void test1() {
  Serial.println("test1");
  flag1 = true;
}


void test2() {
  Serial.println("test2");
  flag2 = true;
}

void setup() {
  Serial.begin(9600);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DAT, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), test, RISING);
  //attachInterrupt(digitalPinToInterrupt(DAT), test, RISING);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.println("Hello Arduino!");
}

bool on = false;

void _loop() {
  if (on) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  }
  on = !on;
  delay(500);
}

void loop() {
  if (COUNT == 0) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  } else if (COUNT == 1) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  } else if (COUNT == 2) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  } else {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }
  flag1 = false;
  flag2 = false;
  delay(10);
}



void RoteStateChanged()  //When CLK  FALLING READ DAT
{
  if (digitalRead(DAT))  // When DAT = HIGH IS FORWARD
  {
    COUNT++;
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(20);
  } else  // When DAT = LOW IS BackRote
  {
    COUNT--;
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
    delay(20);
  }
}

#endif