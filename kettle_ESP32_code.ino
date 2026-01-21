#define BLYNK_TEMPLATE_ID "YOUR TEMPLATE ID"
#define BLYNK_TEMPLATE_NAME "YOUR TEMPLATE NAME"
#define BLYNK_AUTH_TOKEN "INPUT YOUR AUTH TOKEN HERE"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char ssid[] = "YOUR WIFI SSID/NAME";
char pass[] = "WIFI PASSWORD";

#define RELAY_PIN 19
#define DHT_PIN   18
#define RAIN_PIN  5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dht;

unsigned long lastRead = 0;
const unsigned long dhtInterval = 1000;

unsigned long relayOffTime = 0;

bool manualActive = false;
bool manualState  = false;

bool timerActive = false;

bool tempActive = false;
int  tempSetpoint = 0;
float lastTemp = NAN;

bool rainDetected = false;

bool splashDone = false;
unsigned long splashStart = 0;

bool showTempMsg = false;
unsigned long tempMsgStart = 0;
bool tempAchievedShown = false;

BLYNK_WRITE(V0) {
  manualState = param.asInt();
  manualActive = true;
  timerActive = false;
  tempActive = false;
}

BLYNK_WRITE(V2) {
  int minutes = param.asInt();
  if (minutes > 0) {
    relayOffTime = millis() + (unsigned long)minutes * 60000UL;
    timerActive = true;
    manualActive = false;
    tempActive = false;
  } else {
    timerActive = false;
  }
}

BLYNK_WRITE(V3) {
  int segments = param.asInt();
  if (segments > 0) {
    tempSetpoint = segments * 10;
    tempActive = true;
    manualActive = false;
    timerActive = false;
    showTempMsg = true;
    tempMsgStart = millis();
    tempAchievedShown = false;
  } else {
    tempActive = false;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  pinMode(RAIN_PIN, INPUT_PULLUP);
  dht.setup(DHT_PIN, DHTesp::DHT11);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 8);
  display.println("Smart IoT");
  display.setCursor(25, 35);
  display.println("Kettle");
  display.display();

  splashStart = millis();

  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {
  Blynk.run();

  if (!splashDone) {
    if (millis() - splashStart >= 5000) {
      splashDone = true;
      display.clearDisplay();
      display.display();
    } else {
      return;
    }
  }

  bool rainNow = (digitalRead(RAIN_PIN) == LOW);
  if (rainNow) {
    rainDetected = true;
    digitalWrite(RELAY_PIN, HIGH);
  } else if (rainDetected && !rainNow) {
    rainDetected = false;
  }

  if (millis() - lastRead >= dhtInterval) {
    lastRead = millis();
    TempAndHumidity data = dht.getTempAndHumidity();
    if (!isnan(data.temperature)) {
      lastTemp = data.temperature;
      Blynk.virtualWrite(V1, lastTemp);

      if (tempActive && !tempAchievedShown && lastTemp >= tempSetpoint) {
        showTempMsg = true;
        tempMsgStart = millis();
        tempAchievedShown = true;
      }
    }
  }

  bool relayShouldBeOn = false;

  if (!rainDetected) {
    if (tempActive && !isnan(lastTemp)) {
      relayShouldBeOn = (lastTemp < tempSetpoint);
    }
    else if (timerActive) {
      if (millis() < relayOffTime) {
        relayShouldBeOn = true;
      } else {
        timerActive = false;
        Blynk.virtualWrite(V2, 0);
        Blynk.virtualWrite(V0, 0);
      }
    }
    else if (manualActive) {
      relayShouldBeOn = manualState;
    }
  }

  digitalWrite(RELAY_PIN, relayShouldBeOn ? LOW : HIGH);

  // ===== OLED DISPLAY (ONE SCREEN ONLY) =====
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print("Temp: ");
  if (!isnan(lastTemp)) {
    display.print(lastTemp);
    display.print(" C");
  } else {
    display.print("--");
  }

  display.setCursor(0, 17);
  display.print("Relay: ");
  display.print(relayShouldBeOn ? "ON" : "OFF");

  display.setCursor(0, 32);
  if (timerActive && millis() < relayOffTime) {
    unsigned long remaining = (relayOffTime - millis()) / 1000;
    display.print("Time left: ");
    display.print(remaining);
    display.print("s");
  } else {
    display.print("Time left: --");
  }

  display.setCursor(0,47);
  if (showTempMsg && millis() - tempMsgStart < 4000) {
    if (!tempAchievedShown) {
      display.print("Temp set: ");
      display.print(tempSetpoint);
      display.print("C");
    } else {
      display.print("Temp ");
      display.print(tempSetpoint);
      display.print("C reached");
    }
  } else {
    showTempMsg = false;
    if (tempActive) {
      display.print("Threshold: ");
      display.print(tempSetpoint);
      display.print("C");
    } else {
      display.print("Threshold: --");
    }
  }

  display.display();
}
