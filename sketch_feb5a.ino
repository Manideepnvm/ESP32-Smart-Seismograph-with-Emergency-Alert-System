#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>

// -------- OLED CONFIG --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
#define OLED_RESET -1

// -------- PIN CONFIG --------
#define SDA_PIN 21
#define SCL_PIN 22
#define BUZZER_PIN 25
#define LED_PIN 26

// -------- WIFI --------
#define WIFI_SSID "wifi username"
#define WIFI_PASSWORD "wifi password"

// -------- TWILIO CONFIG (PASTE YOUR KEYS HERE) --------
#define TWILIO_ACCOUNT_SID "AC9372c00xxxxxxxxxxxxxxxxxd2d43"
#define TWILIO_AUTH_TOKEN "0f43befc9xxxxxxxxxxxxxxxxxc2650"
#define TWILIO_PHONE_NUMBER "+15xxxxxxx804"
#define YOUR_PHONE_NUMBER "xxxxxxxxxxxxxxx"   

// -------- LOGIC SETTINGS --------
#define VIBRATION_THRESHOLD 1.8
#define HIT_COOLDOWN 5000       // 5 seconds (Wait this long before counting a new hit)
#define COUNTER_TTL 60000       // 60 seconds (If quiet for 60s, reset counter to 0)

// -------- OBJECTS --------
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu;

// -------- VARIABLES --------
int earthquakeCount = 0;           // Tracks number of hits
unsigned long lastTriggerTime = 0; // Tracks time of last valid hit

// ================= SEND SMS =================
void sendTwilioSMS(String message) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.twilio.com/2010-04-01/Accounts/" +
               String(TWILIO_ACCOUNT_SID) + "/Messages.json";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String auth = String(TWILIO_ACCOUNT_SID) + ":" + String(TWILIO_AUTH_TOKEN);
  String encodedAuth = base64::encode(auth);
  http.addHeader("Authorization", "Basic " + encodedAuth);

  String body = "To=" + String(YOUR_PHONE_NUMBER) +
                "&From=" + String(TWILIO_PHONE_NUMBER) +
                "&Body=" + message;

  int httpCode = http.POST(body);
  http.end();

  Serial.print("SMS Status: ");
  Serial.println(httpCode);
}

// ================= MAKE CALL =================
void makeTwilioCall() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.twilio.com/2010-04-01/Accounts/" +
               String(TWILIO_ACCOUNT_SID) + "/Calls.json";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String auth = String(TWILIO_ACCOUNT_SID) + ":" + String(TWILIO_AUTH_TOKEN);
  String encodedAuth = base64::encode(auth);
  http.addHeader("Authorization", "Basic " + encodedAuth);

  // We use the 'Twiml' parameter to define what the call says without needing an external XML file
  String speech = "<Response><Say>Emergency! Earthquake detected. Level 3 Alert.</Say></Response>";
  
  String body = "To=" + String(YOUR_PHONE_NUMBER) +
                "&From=" + String(TWILIO_PHONE_NUMBER) +
                "&Twiml=" + speech;

  int httpCode = http.POST(body);
  http.end();

  Serial.print("Call Status: ");
  Serial.println(httpCode);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED
  display.begin(OLED_ADDRESS, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Seismograph");
  display.setCursor(10, 35);
  display.println("System Ready");
  display.display();

  // MPU6050
  mpu.initialize();

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected");
  delay(1000);
}

// ================= LOOP =================
void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float x = ax / 16384.0;
  float y = ay / 16384.0;
  float z = az / 16384.0;

  float vibration = sqrt(x * x + y * y + z * z);

  // OLED Display Logic
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Hits: "); 
  display.print(earthquakeCount);
  display.print("/3");
  
  display.setCursor(0, 15);
  display.println("Vibration Level:");
  display.setTextSize(2);
  display.setCursor(10, 35);
  display.print(vibration, 2);
  display.display();

  // -------- ALERT LOGIC --------
  if (vibration >= VIBRATION_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);

    unsigned long currentMillis = millis();

    // 1. Debounce Check: Has enough time passed since the last count?
    if (currentMillis - lastTriggerTime > HIT_COOLDOWN) {
      
      // 2. TTL Check: If it's been too long since the last hit, reset counter
      if (currentMillis - lastTriggerTime > COUNTER_TTL) {
        earthquakeCount = 0;
        Serial.println("TTL Expired. Counter Reset.");
      }

      // 3. Increment Counter
      earthquakeCount++;
      lastTriggerTime = currentMillis;
      Serial.print("Vibration detected! Count: ");
      Serial.println(earthquakeCount);

      // 4. Action Decision
      if (earthquakeCount < 3) {
        sendTwilioSMS("Alert " + String(earthquakeCount) + "/3: Vibration Detected!");
      } else {
        makeTwilioCall();
        // Optional: Reset count after call so it doesn't spam calls continuously?
        // earthquakeCount = 0; 
      }
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}