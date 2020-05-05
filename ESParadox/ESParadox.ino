#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <ESP8266httpUpdate.h>

#include "paradox_constants.h"
#include "private.h"
#include "mqtt.h"
#include "paradox.h"

BearSSL::WiFiClientSecure net;
MQTTClient client(256);
time_t now;

void setup()
{
  // init serial for Paradox (UART0)
  Serial.begin(9600);
  Serial.flush();
  // move Paradox on UART1
  Serial.swap();
  // flush serial buffer
  flush_serial_buffer();

  // initialize wifi
  init_wifi();
  // get ntp time
  ntp_time();
  // take care of certs
  certification();
  // setup & connect mqtt
  mqtt_setup();

  // flush serial buffer before loop
  flush_serial_buffer();
}

void loop()
{
  if (!client.connected())
    mqtt_connect();

  client.loop();
  mqtt_loop();
  paradox_loop();
}

void flush_serial_buffer()
{
  Serial.flush();
  while (Serial.read() >= 0)
    yield();
}