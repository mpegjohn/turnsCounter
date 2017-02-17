#include <LiquidCrystal.h>

#define LCD_D7 A0
#define LCD_D6 A1
#define LCD_D5 A2
#define LCD_D4 A3
#define LCD_EN 12
#define LCD_RS 11

// the push button on the encoder
#define buttonPin 4
#define PULSE_INPUT_PIN 2

#define LCD_COLS 20
#define LCD_ROWS 4


int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// initialize the lcd library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

volatile unsigned long count = 0;

void updateCount() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // Only update count if it has been longer than 10mS
  if(interrupt_time - last_interrupt_time > 10)
  {
    count++;
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  
  lcd.setCursor(0,1);
  lcd.print("Turns: ");
  
  // setup interupt for encoder
  attachInterrupt(0, updateCount, CHANGE);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        count = 0;
      }
    }
  }
  
  delay(200);
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
  lcd.setCursor(8, 1);
  
  float actual_turns = (float)count/24.0;
  
  lcd.print("       ");
  lcd.setCursor(8, 1);
  
  lcd.print(actual_turns, 1);
}

