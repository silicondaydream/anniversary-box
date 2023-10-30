#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;

// Define LCD pinouts
const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;

// Define I2C Adress - change if needed
const int i2c_addr = 0x27;

// Initialize the LCD object with the address of the display
LiquidCrystal_I2C lcd(0x27, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE); 

// Set the predetermined date (date you met)
DateTime metDate(2016, 10, 31, 0, 0, 0);

// Joystick pins
const int joystickPinX = A0; 
const int joystickPinY = A1; 
int currentMode = 4;

void setup() {
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  if (!rtc.isrunning()) {
    lcd.print("RTC not running!");
    delay(2000);
    // Uncomment the following line to set the RTC to the current date and time
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}


void loop() {
  DateTime now = rtc.now();
  long difference = now.unixtime() - metDate.unixtime();

  int joystickValueX = analogRead(joystickPinX);
  int joystickValueY = analogRead(joystickPinY);
  
  // Determine the direction based on joystick values
  if (joystickValueX < 200) { 
    currentMode = 0; // Left
  } else if (joystickValueX > 500) { 
    currentMode = 1; // Right
  } else if (joystickValueY < 200) { 
    currentMode = 2; // Up
  } else if (joystickValueY > 500) { 
    currentMode = 3; // Down
  } else {
    currentMode = 4; // Centered
  }

  lcd.clear();
  switch(currentMode) {
    case 0:
      lcd.print("Min. together:");
      lcd.setCursor(0, 1);
      lcd.print(difference / 60);
      break;
    case 1:
      lcd.print("Hours together:");
      lcd.setCursor(0, 1);
      lcd.print(difference / 3600);
      break;
    case 2:
      lcd.print("Days together:");
      lcd.setCursor(0, 1);
      lcd.print(difference / 86400);
      break;
    case 3:
      lcd.print("Months together:");
      lcd.setCursor(0, 1);
      lcd.print((difference / 86400) / 30); // Approximating month as 30 days
      break;
    case 4:
      lcd.print("Years together:");
      lcd.setCursor(0, 1);
      lcd.print((difference / 86400) / 365); // Approximating year as 365 days
      break;
  }
  
  delay(200); // Debounce and refresh rate
}