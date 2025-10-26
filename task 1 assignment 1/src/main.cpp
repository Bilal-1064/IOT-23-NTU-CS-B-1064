//Muhammad Bilal
//23-NTU-CS-1064
//BSCS-5th-B
//use one button to cycle through led modes display the current state on the OLED


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions - CONNECTIONS
#define BUTTON_MODE_PIN 2
#define BUTTON_RESET_PIN 3
#define LED1_PIN 5
#define LED2_PIN 6
#define LED3_PIN 9
#define BUZZER_PIN 10

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Program variables
int currentMode = 0;  // Start with Mode 0: Both OFF
const int TOTAL_MODES = 4;
unsigned long previousBlinkTime = 0;
bool blinkState = false;
int fadeValue = 0;
bool fadeDirection = true;

// Mode names for display
const char* modeNames[] = {
  "Both OFF",
  "Alternate Blink", 
  "Both ON",
  "PWM Fade"
};

// ✅ Function Prototypes (added for PlatformIO)
void resetLEDs();
void updateDisplay();
void alternateBlink();
void pwmFade();

void setup() {
  // Initialize pins
  pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // Turn off all LEDs at start
  resetLEDs();
  updateDisplay();
}

void loop() {
  // Check MODE button (cycle through modes)
  if (digitalRead(BUTTON_MODE_PIN) == LOW) {
    currentMode = (currentMode + 1) % TOTAL_MODES;
    resetLEDs();
    updateDisplay();
    delay(300); // Simple debounce
  }
  
  // Check RESET button (go to OFF mode)
  if (digitalRead(BUTTON_RESET_PIN) == LOW) {
    currentMode = 0;
    resetLEDs();
    updateDisplay();
    delay(300);
  }
  
  // Execute the current mode
  switch(currentMode) {
    case 0: // Mode 0: Both OFF
      resetLEDs();
      break;
      
    case 1: // Mode 1: Alternate Blink
      alternateBlink();
      break;
      
    case 2: // Mode 2: Both ON
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, HIGH);
      break;
      
    case 3: // Mode 3: PWM Fade
      pwmFade();
      break;
  }
}

void resetLEDs() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  analogWrite(LED3_PIN, 0);
}

void alternateBlink() {
  unsigned long currentTime = millis();
  if (currentTime - previousBlinkTime >= 500) { // Blink every 500ms
    previousBlinkTime = currentTime;
    blinkState = !blinkState;
    
    digitalWrite(LED1_PIN, blinkState);
    digitalWrite(LED2_PIN, !blinkState);
    digitalWrite(LED3_PIN, blinkState);
  }
}

void pwmFade() {
  unsigned long currentTime = millis();
  if (currentTime - previousBlinkTime >= 20) { // Smooth fade
    previousBlinkTime = currentTime;
    
    // Update fade value
    if (fadeDirection) {
      fadeValue += 5;
      if (fadeValue >= 255) {
        fadeValue = 255;
        fadeDirection = false;
      }
    } else {
      fadeValue -= 5;
      if (fadeValue <= 0) {
        fadeValue = 0;
        fadeDirection = true;
      }
    }
    
    
    analogWrite(LED3_PIN, fadeValue);
    
    digitalWrite(LED1_PIN, fadeValue > 127);
    digitalWrite(LED2_PIN, fadeValue > 127);
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("LED Control System");
  display.println("Task A - Mode Cycle");
  display.println("-------------------");
  display.print("Mode ");
  display.print(currentMode);
  display.print(": ");
  display.println(modeNames[currentMode]);
  display.display();
}