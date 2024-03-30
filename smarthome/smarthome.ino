#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

#define IN_TOPIC   "INRELAY"
#define OUT_TOPIC  "OUTRELAY"

#define IN_TOPIC2  "INRELAY2"
#define OUT_TOPIC2 "OUTRELAY2"

#define IN_TOPIC3  "INRELAY3"
#define OUT_TOPIC3 "OUTRELAY3"

#define IN_TOPIC4  "INRELAY4"
#define OUT_TOPIC4 "OUTRELAY4"


// Update these with values suitable for your network.

//const char* ssid = "TEKNOLAB Office";
//const char* password = "selamatdatang";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

 delay(10);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);

 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }

 randomSeed(micros());

 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

 
  if ((char)payload[0] == '1') {
    digitalWrite(14, HIGH); 
  } else if ((char)payload[0] == 'A'){
    digitalWrite(14, LOW);  
  }
   if ((char)payload[0] == '2'){
    digitalWrite(16,HIGH);
  } else if((char)payload[0] == 'B') {
    digitalWrite(16,LOW);
  }
  if ((char)payload[0] == '3'){
    digitalWrite(12,HIGH);
  }  else if((char)payload[0] == 'C') {
    digitalWrite(12,LOW);
  }
   if ((char)payload[0] == '4'){
    digitalWrite(13,HIGH);
  }  else if((char)payload[0] == 'D') {
    digitalWrite(13,LOW);
  }
 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(IN_TOPIC, "Relay ON");
      // ... and resubscribe
      client.subscribe(IN_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(IN_TOPIC2, "Relay ON");
      // ... and resubscribe
      client.subscribe(IN_TOPIC2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
     if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(IN_TOPIC3, "Relay ON");
      // ... and resubscribe
      client.subscribe(IN_TOPIC3);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(IN_TOPIC4, "Relay ON");
      // ... and resubscribe
      client.subscribe(IN_TOPIC4);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
   pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  //setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
    WiFiManager wm;
    //wm.resetSettings();
    bool res;
    res = wm.autoConnect("Smart HOME","TEKNOLAB"); 
    if(!res) {
        Serial.println("Failed to connect");
    } 
    else { 
        Serial.println("connected...yeey :)");
    }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
