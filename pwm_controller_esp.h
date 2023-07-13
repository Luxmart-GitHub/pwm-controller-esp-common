#ifndef PWM_CONTROLLER_ESP32_H
#define PWM_CONTROLLER_ESP32_H

// Enable this macro to have DBGLOG() outputs into the hardware serial port.
// For example, in case of esp8266 the GPIO1 and GPIO3 should be the Serial TX and RX pins.
// Note: if outputs are not printed into the serial output, make sure BAUD_RATE is correct,
// and reset the device (or its adapter), while keeping the serial console open.
//#define DEBUG

#ifdef DEBUG
// Since debug serial and command serial could use the same serial bus, we prepend
// logging messages with '#' to indicate that subsequent characters
// are not a command. Logging message continues until the carriage return symbol.
#define DBGLOG(msg) do { Serial.write("#"); Serial.write(msg); Serial.write("\n\r"); } while (0)
#else
#define DBGLOG(msg) do { } while (0)
#endif

#ifdef ESP32
#define LED_LOW HIGH
#define LED_HIGH LOW
#include <WiFi.h>
#include <HardwareSerial.h>
#define pwmSerial Serial
#else
#define LED_LOW LOW
#define LED_HIGH HIGH
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#define RX_PIN 1
#define TX_PIN 3
SoftwareSerial pwmSerial(RX_PIN, TX_PIN);
#endif

#include <WiFiClientSecure.h>

static void setupSerial()
{
  Serial.begin(BAUD_RATE);
  if (pwmSerial != Serial)
    pwmSerial.begin(BAUD_RATE);
}

#define WIFI_TIMEOUT 10000

IPAddress localIP;
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

static bool setupWiFi()
{
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());

  if (!WiFi.config(localIP, localGateway, subnet)){
    DBGLOG("STA Failed to configure");
    return false;
  }

  DBGLOG("Trying to connect to:");
  DBGLOG(ssid.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      return false;
    }
    
    yield();
  }

  return WiFi.status() == WL_CONNECTED;
}

#endif // PWM_CONTROLLER_ESP32_H
