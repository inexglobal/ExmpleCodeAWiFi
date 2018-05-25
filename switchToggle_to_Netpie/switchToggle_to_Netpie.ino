/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = "SSID";
const char* password = "1234546789-0";

#define APPID   "GroupInex"
#define KEY     "Yn1yT7QqyKyYFk6"
#define SECRET  "Ksn1hYJ2ELSNjsKb2iiJa6YIg"
#define ALIAS   "ESPID_01"

WiFiClient client;

int timer = 0;
int swPin = D3;
int ledPin = D5;
bool statusLed, lastStatusLed;

MicroGear microgear(client);

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

  pinMode(swPin, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(swPin, toggle, FALLING); 
  
  /* Add Event listeners */
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);
  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  unsigned long timeoutconnect = millis();
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (millis() - timeoutconnect > 8000) {
        break;
      }
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}
void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    Serial.println("connected");
    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();
    
    if (lastStatusLed != statusLed) { //จะส่งก็ต่อเมื่อเกิดการเปลี่ยนแปลงสถานะ LED 
      microgear.chat("ESPID_02/LED", statusLed); //ส่งสถานะ LED
      lastStatusLed = statusLed;
    }
  }
  else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}
void toggle() {
  statusLed = !statusLed;
  digitalWrite(ledPin, statusLed);
}

