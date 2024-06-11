#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD pin connections (RS, E, D4, D5, D6, D7)
const int buzzerPin = 8; // Pin for the buzzer
const int joyXPin = A1;  // Pin for Joystick X-axis
const int joyYPin = A2;  // Pin for Joystick Y-axis

unsigned long previousMillis = 0; // Variable to store the previous time
unsigned long currentMillis = 0;  // Variable to store the current time
const long interval = 1000;  // 1 second (1000 milliseconds)

int hours = 0;   // Hours
int minutes = 0; // Minutes
int seconds = 0; // Seconds
bool countdownActive = false; // Countdown state

void setup() {
  lcd.begin(16, 2); // Initialize the LCD
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output
}

void loop() {
  int joyX = analogRead(joyXPin); // Read the Joystick X-axis value
  int joyY = analogRead(joyYPin); // Read the Joystick Y-axis value

  lcd.setCursor(0, 0); // Move to the first line
  if (joyX < 100) {
    lcd.clear();
    lcd.print("Counting Down!");
    if (!countdownActive) {
      startCountdown(hours, minutes, seconds);
    }
  } else if (joyX > 900) {
    lcd.print("Added 1 minute");
    minutes++;
  } else if (joyY < 100) {
    lcd.print("Added 1 hour");
    hours++;
  } else if (joyY > 900) {
    lcd.print("Added 1 second");
    seconds++;
  }

  updateDisplay();
  delay(200); // Add a short delay to prevent rapid changes on the screen
}

void startCountdown(int startHours, int startMinutes, int startSeconds) {
  hours = startHours;
  minutes = startMinutes;
  seconds = startSeconds;
  countdownActive = true;
}

void updateDisplay() {
  lcd.setCursor(0, 1); // Move to the second line
  lcd.print(hours < 10 ? "0" + String(hours) : String(hours));
  lcd.print(":");
  lcd.print(minutes < 10 ? "0" + String(minutes) : String(minutes));
  lcd.print(":");
  lcd.print(seconds < 10 ? "0" + String(seconds) : String(seconds));

  if (countdownActive) {
    currentMillis = millis();  // Get the current time
    if (currentMillis - previousMillis >= interval) {  // Check at specified intervals
      if (seconds > 0) {
        seconds--;
      } else {
        if (minutes > 0) {
          minutes--;
          seconds = 59;
        } else {
          if (hours > 0) {
            hours--;
            minutes = 59;
            seconds = 59;
          } else {
            countdownActive = false;  // Stop the countdown
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Countdown Over!");
            tone(buzzerPin, 1000, 1000); // Sound the buzzer for 1 second
          }
        }
      }
      previousMillis = currentMillis;  // Update the previous time
    }
  }
}
