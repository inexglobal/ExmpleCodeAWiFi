/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = "SSID";
const char* password = "Password";

#define APPID   "GroupInex"
#define KEY     "Yn1yT7QqyKyYFk6"
#define SECRET  "Ksn1hYJ2ELSNjsKb2iiJa6YIg"
#define ALIAS   "ESPID_01"

WiFiClient client;
int timer = 0;
int pinLED_B = D0;
int pinLED_G = D1;
int pinLED_R = D2;
int colorR = 500;
int colorG = 500;
int colorB = 500;

MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  String color;
  color.concat((char)msg[1]);
  color.concat((char)msg[2]);
  color.concat((char)msg[3]);
  color.concat((char)msg[4]);
  color.trim();

  if (msg[0] == 'R') {
    Serial.println("color R =" + color);
    colorR = color.toInt();
    analogWrite(pinLED_R, colorR);
    microgear.chat("ESPID_02/LED/R", colorR);
  } else if (msg[0] == 'G') {
    Serial.println("color G =" + color);
    colorG = color.toInt();
    analogWrite(pinLED_G, colorG);
    microgear.chat("ESPID_02/LED/G", color);
  } else if (msg[0] == 'B') {
    Serial.println("color B =" + color);
    colorB = color.toInt();
    analogWrite(pinLED_B, colorB);
    microgear.chat("ESPID_02/LED/B", color);
  } else if (msg[0] == 'r') {
    microgear.chat("ESPID_02/LED/R", colorR);
  } else if (msg[0] == 'g') {
    microgear.chat("ESPID_02/LED/G", colorG);
  } else if (msg[0] == 'b') {
    microgear.chat("ESPID_02/LED/B", colorB);
  }
}
/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(ALIAS);
}
void setup() {


  pinMode(pinLED_R, OUTPUT);
  pinMode(pinLED_G, OUTPUT);
  pinMode(pinLED_B, OUTPUT);
  analogWrite(pinLED_R, colorR);
  analogWrite(pinLED_G, colorG);
  analogWrite(pinLED_B, colorB);
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
}
void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    Serial.println("connected");
    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();
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


