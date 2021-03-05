#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "TURBONETT_523"
#define WLAN_PASS       "a4c564b0c0"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "men18300"
#define AIO_KEY         "aio_jdTp83i1XFRIGRVXa8wFhdnx1bGO"

#define BUTTON1   35
#define BUTTON2   0
#define LED_GREEN 19
#define LED_RED   21  

#define FF17 &FreeSans9pt7b
#define FF21 &FreeSansBold9pt7b
#define ROW1 0,16
#define ROW2 0,38
#define ROW3 0,60
#define ROW4 0,82
#define ROW5 0,104
#define ROW6 0,126


WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe ledControl = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fucks");
Adafruit_MQTT_Subscribe ledVerde = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ledverde");
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

void setup()
{
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  Serial.begin(115200);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);


  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


  
  mqtt.subscribe(&ledControl);
  mqtt.subscribe(&ledVerde);
}

float ambientCelsius = 0.0;
float objectCelsius = 0.0;

void loop()
{
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &ledControl) {
      Serial.print(F("Got: "));
      Serial.println((char *)ledControl.lastread);

      if (!strcmp((char*) ledControl.lastread, "ON")) {
        digitalWrite(LED_GREEN, HIGH);
        Serial.write(1);

      }
      else {
        digitalWrite(LED_GREEN, LOW);
        Serial.write(2);
      }
    }
    else if (subscription == &ledVerde) {
      Serial.print(F("Got: "));
      Serial.println((char *)ledVerde.lastread);

      if (!strcmp((char*) ledVerde.lastread, "ON")) {
        digitalWrite(LED_RED, HIGH);
        Serial.write(3);

      }
      else {
        digitalWrite(LED_RED, LOW);
        Serial.write(4);
      }
    }


    
  }

          int conteo= Serial.read();
        Serial.print(conteo);

  ambientCelsius = conteo;
  objectCelsius = conteo;
  Serial.print(F("\nSending temperature val "));
  Serial.print(conteo);
  Serial.print("…");
  
  if (!temperature.publish(objectCelsius)) {
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("OK!"));
  }
}

void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT… ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds…");
    mqtt.disconnect();

    delay(5000); // wait 5 seconds

    -retries;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}
