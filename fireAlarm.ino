#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#define flameSensor D1
#define resButton D3
#define outPin D2

const char* ssid = ""; // SSID
const char* password = ""; // Password

float ct = 0;
float ot = 0;
float dt = 0;
float timer = 0;
float temp = 0;
bool active = false;
bool active2 = false;

WebSocketsServer webSocket = WebSocketsServer(80); // port

void setup() {
  Serial.begin(9600);
  
  pinMode(flameSensor, INPUT);
  pinMode(resButton, INPUT);
  pinMode(outPin, OUTPUT);

  WiFi.begin(ssid, password);
  while(true){
    if(WiFi.status() == WL_CONNECTED){
      break;
    }
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
  
  Serial.println("");
  Serial.println("WiFi connected");
  delay(2000);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  delay(1000);
  ct = millis();
}

void loop(){
    ot = ct;
    ct = millis();
    dt = ct - ot;
    active2 = active;
    webSocket.loop();
    int analogValue = analogRead(A0);
    float millivolts = (analogValue) * (3300.0 / 1024.0);
    temp = millivolts / 10;

  if(digitalRead(flameSensor) == LOW){
        active = true;
  } else {
        active = false;
  }


    if(active == true || active2 == true){
          timer = timer + dt;
    } else {
          timer = 0;
    }

    if(timer >= 2000){
        digitalWrite(outPin, HIGH);
        active = true;
    } else {
        digitalWrite(outPin, LOW);
    }

    if(digitalRead(resButton) == LOW){
        active = false;
        active2 = false;
        timer = 0;
        webSocket.broadcastTXT("fireOff");
    }

    if(active == true){
        delay(500);
        webSocket.broadcastTXT("fire");
    } else {
        digitalWrite(outPin, LOW);
        delay(150);
    }

    // delays are ussually used to prevent esp8266 to draw much power while using wifi.
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] Received text: %s\n", num, payload);
            if(String((char*)payload) == "extinguish"){ // turn off broadcasting
                active = false;
                active2 = false;
                timer = 0;
                webSocket.sendTXT(num, "setTo0");
            } else if(String((char*)payload) == "temp"){ // get temperature (Celcius)
                String message = "t: " + String(temp);
                webSocket.sendTXT(num, message);
            } else {
                webSocket.sendTXT(num, "works");
            }
            break;
    }
}
