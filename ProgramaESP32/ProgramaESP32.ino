#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "TURBONETT_F895FB"
#define WLAN_PASS       "795C90366B"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "men18300"
#define AIO_KEY         "aio_jdTp83i1XFRIGRVXa8wFhdnx1bGO"

#define BUTTON1   35
#define BUTTON2   0
#define LED_GREEN 19
#define LED_RED   21
#define RXD2 16
#define TXD2 17

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


//////////////////////////////////////////////////////////////////////////////////////////////
Adafruit_MQTT_Publish AxADAFRUIT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Ax");
Adafruit_MQTT_Publish AyADAFRUIT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Ay");
Adafruit_MQTT_Publish AzADAFRUIT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Az");
////////////////////////////////////////////////////////////////////////////////////////////////

//HardwareSerial Serial2(2);
void setup()
{
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.begin(115200);


  while (!Serial);
  while (!Serial2);

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
        Serial2.write(0x0D);

      }
      else {
        digitalWrite(LED_GREEN, LOW);
        Serial2.write(0x0C);
      }
    }
    else if (subscription == &ledVerde) {
      Serial.print(F("Got: "));
      Serial.println((char *)ledVerde.lastread);

      if (!strcmp((char*) ledVerde.lastread, "ON")) {
        digitalWrite(LED_RED, HIGH);
        Serial2.write(57);

      }
      else {
        digitalWrite(LED_RED, LOW);
        Serial2.write(58);
      }
    }



  }

  ///////////////////////////////////////////////
  //Intentando comunicarme con el PIc

  //while (Serial2.available()>0) {
  //    int conteo=Serial2.read();
  //    char conteo1=Serial2.read();
  //    Serial.print("\n  Recibiendo conteo->");
  //    Serial.print(conteo);
  //    Serial.print("\n  Recibiendo conteo1->");
  //    Serial.print(conteo1);
  //    delay(200);
  //  }

  //////////////////////////////////////////////////

  //    Serial.write(48);
  //    delay(2000);
  //    Serial.write(49);
  //    delay(2000);
  //    Serial.write(50);
  //    delay(2000);
  //    Serial.write(51);
  //    delay(2000);
  //    Serial.write(52);
  //    delay(2000);

  //
  //while (Serial1.available()>0) {
  //    int conteo=Serial1.read();
  //    Serial.print("\n  Recibiendo conteo->");
  //    Serial.print(conteo);
  //    delay(200);
  //  }



  //////////////////////////////////////////////////////////
 // int conteo = Serial2.read();
  //   Serial.print(conteo);
  //   delay (200);

//  ambientCelsius = conteo;
 // objectCelsius = conteo;


  Serial.print(F("\nSending Aceloremeter value "));
  int conteo=Serial2.read();
  Serial.print(conteo);
  delay (200);
  Serial.print("…");

  /////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////
  //Intentando comunicarme con el PIc parte 2

  //while (Serial2.available()>0) {
  //    int conteo=Serial2.read();
  //    char conteo1=Serial2.read();
  //    Serial.print("\n  Recibiendo conteo->");
  //    Serial.print(conteo);
  //    Serial.print("\n  Recibiendo conteo1->");
  //    Serial.print(conteo1);
  //    delay(200);
  //  }

  //////////////////////////////////////////////////


//  while (1){
//    if (Serial2.available()>0){
//  Serial.print((Serial2.read()));
//  Serial.print("\n");
//  }}

  /////////////////////////////////////////////////////
  if (conteo == 6) {
    int Ax = Serial2.read();
    if (!AxADAFRUIT.publish(Ax)) {
      Serial.println(F("Failed sending Ax"));
    }
    else {
      Serial.println(F("OK! sending Ax "));
      Serial.print(Ax);
    }



  }
  else if (conteo == 24) {
    int Ay = Serial2.read();
    if (!AyADAFRUIT.publish(Ay)) {
      Serial.println(F("Failed sending Ay" ));
    }
    else {
      Serial.println(F("OK! sending Ay"));
      Serial.print(Ay);
    }




  }
  else if (conteo == 30) {
    int Az = Serial2.read();
    if (!AzADAFRUIT.publish(Az)) {
      Serial.println(F("Failed sending Az"));
      
    }
    else {
      Serial.println(F("OK! sending Az"));
      Serial.print(Az);
    }



  }

  ///////////////////////////////////////////////////////////////////
  //
  //    if (!temperature.publish(objectCelsius)) {
  //      Serial.println(F("Failed"));
  //    }
  //    else {
  //      Serial.println(F("OK!"));
  //    }
  //
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
