/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "DHT.h"

const char* ssid     = "SSID";
const char* password = "123456789-0";

#define APPID   "GroupInex"
#define KEY     "Yn1yT7QqyKyYFk6"
#define SECRET  "Ksn1hYJ2ELSNjsKb2iiJa6YIg"
#define ALIAS   "ESPID_01"

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 15);
WiFiClient client;

int timer = 0;
MicroGear microgear(client);
unsigned long previousMillis = 0;
/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}
/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(ALIAS);
}
void setup() {
  /* Add Event listeners */
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);
  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);
  Serial.begin(115200);
  Serial.println("Starting...");
  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);
  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
  dht.begin();
}
void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 15000) {
      previousMillis = currentMillis;
      Serial.print("Write Feed..., Data = ");
      /* Chat with the microgear named ALIAS which is myself */
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        String data = "temp:{t},humid:{h}";
        data.replace("{t}", String(t));
        data.replace("{h}", String(h));
        Serial.println(data);
        microgear.writeFeed("MyFeedWeather",data);
      }
    }
  }
  else {
    if (timer >= 5000) {
      Serial.println("connection lost, reconnect...");
      microgear.connect(APPID);
      timer = 0;
    }
    else timer++;
  }
}
