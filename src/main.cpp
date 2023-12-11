#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "It should be either ESP32 or ESP8266 device!"
#endif

#undef ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define PRINT(...) Serial.print(__VA_ARGS__);
#define PRINTLN(...) Serial.println(__VA_ARGS__);
#define PRINTF(f, ...) Serial.printf(f, __VA_ARGS__);
#else
#define PRINT(...) ;
#define PRINTLN(...) ;
#define PRINTF(f, ...) ;
#endif

#include <HttpClient.h>
#include <WiFiClientSecure.h>
#include <TelegramClient.h>

#include "credentials.h"

#define POWER_PIN 5

WiFiClientSecure wifiClient;
TelegramClient telegram(wifiClient);

bool wifi_init()
{
  WiFi.begin(SSID.c_str(), PASSWORD.c_str());
  uint8_t retry_count = 0;
  PRINTLN("");
  while (WiFi.status() != WL_CONNECTED && retry_count++ < 60)
  {
    delay(200);
    PRINT("\r");
    PRINT(retry_count);
    PRINT("  ");
  }
  PRINTLN("");
  return WiFi.status() == WL_CONNECTED;
}

void setup()
{
#ifdef ENABLE_DEBUG
  Serial.begin(115200);
  while (!Serial)
    ;
  PRINTLN();
  PRINTLN("Serial is ready");
#endif
  PRINTLN("Enable power");
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  delay(100);
  PRINT("Connecting Wifi...");
  if (wifi_init())
  {
    PRINTLN();
    PRINT("Local IP:");
    PRINTLN(WiFi.localIP());
    HttpClient client = HttpClient(wifiClient, "api.telegram.org", 443);
    wifiClient.setInsecure();
    telegram.setToken(API_KEY);
    String data = "{\"chat_id\": " + CHAT_ID + ", \"text\": \"" + NODE_ID + " ALARM!!!\"}";
    client.post("/bot6348567022:AAHIrXZMrVrM-B3UzDTgo6IS7Xere7KH_cY/sendMessage", "application/json", data);
    // telegram.sendMessage(CHAT_ID.toInt(), NODE_ID + " ALARM!!!");
  }

  PRINTLN("Disable power");
  PRINT("Go to sleep...");
  delay(100);
  digitalWrite(POWER_PIN, LOW);
}

void loop()
{
}