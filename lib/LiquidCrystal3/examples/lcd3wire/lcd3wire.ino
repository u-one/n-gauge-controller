#include <LiquidCrystal3.h>

// LiquidCrystal lcd(clock, data, latch);
// d0-d13,a0-a5 select free!
LiquidCrystal lcd(14, 15, 16);

void setup() {
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("TIME:");
  if (((millis() / 1000) % 100) < 10)
    lcd.print("_");
  lcd.print((millis() / 1000) % 100);
  lcd.print(".");
  lcd.print((millis() / 100) % 10);
}

