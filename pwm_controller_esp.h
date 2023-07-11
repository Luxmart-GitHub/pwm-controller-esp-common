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

static void setupWiFi()
{
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  pinMode(LED_BUILTIN, OUTPUT);
  auto state = LED_LOW;
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, state);
    delay(500);
    state ^= LED_HIGH;
    digitalWrite(LED_BUILTIN, state);
    delay(500);
    state ^= LED_HIGH;
  }
}

#endif // PWM_CONTROLLER_ESP32_H
