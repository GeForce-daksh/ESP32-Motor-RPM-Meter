#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 23
#define OLED_CLK  18
#define OLED_DC   16
#define OLED_CS   5
#define OLED_RST  4

#define HALL_PIN 27

// Change to 2 if using two magnets
const int MAGNETS_PER_REV = 1;

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RST, OLED_CS
);

volatile unsigned long pulseCount = 0;
volatile unsigned long lastPulseUs = 0;

unsigned long lastUpdate = 0;
unsigned long maxRPM = 0;
float rpm = 0;

const unsigned long UPDATE_INTERVAL = 500;
const unsigned long STOP_TIMEOUT_US = 1500000;

// Hall sensor output is commonly active LOW
void IRAM_ATTR hallISR() {
  unsigned long now = micros();

  // Ignore false pulses closer than 200 us
  if (now - lastPulseUs >= 200) {
    pulseCount++;
    lastPulseUs = now;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(HALL_PIN, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(HALL_PIN),
    hallISR,
    FALLING
  );

  SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED initialization failed!");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 20);
  display.println("MOTOR RPM METER");
  display.display();

  delay(1000);
  lastUpdate = millis();
}

void loop() {
  unsigned long now = millis();

  if (now - lastUpdate >= UPDATE_INTERVAL) {
    unsigned long elapsed = now - lastUpdate;
    lastUpdate = now;

    unsigned long pulses;
    unsigned long lastPulse;

    noInterrupts();
    pulses = pulseCount;
    pulseCount = 0;
    lastPulse = lastPulseUs;
    interrupts();

    // RPM from pulses measured during this interval
    rpm = (pulses * 60000.0) /
          (elapsed * MAGNETS_PER_REV);

    // Show zero if no recent pulse was detected
    if (lastPulse == 0 ||
        micros() - lastPulse > STOP_TIMEOUT_US) {
      rpm = 0;
    }

    if (rpm > maxRPM) {
      maxRPM = (unsigned long)rpm;
    }

    Serial.print("RPM: ");
    Serial.println((unsigned long)rpm);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("MOTOR RPM METER");

    display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

    display.setCursor(0, 19);
    display.println("CURRENT RPM");

    display.setTextSize(3);
    display.setCursor(0, 29);
    display.println((unsigned long)rpm);

    display.setTextSize(1);
    display.setCursor(0, 56);
    display.print("MAX: ");
    display.print(maxRPM);

    display.display();
  }
}