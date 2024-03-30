#include <HTTPClient.h> 
#include <WiFi.h> 
#include <SoftwareSerial.h> 
const int pinrelay = 4; 
 
const char *ssid = "111"; 
const char *password = "11111"; 
 
const char* url = "https://doorlock.my.id/update_switch.php"; 
#include <Arduino.h> 
#if defined(ESP32) 
#elif defined(ESP8266) 
#include <ESP8266WiFi.h> 
#endif 
SoftwareSerial Esp32(3, 1); 
 
void setup() { 
  // put your setup code here, to run once: 
  pinMode(pinrelay, OUTPUT); 
  Serial.begin(9600); 
  WiFi.hostname("Esp 32"); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) 
  { 
    Serial.print("."); 
    delay(500); 
  } 
 
  Serial.print("Terkoneksii"); 
 
} 
 
void loop() { 
  WiFiClient client; 
  const int httpPort = 80; 
  if (!client.connect(url, httpPort)) 
  { 
    Serial.println("Gagal Koneksi ke Server"); 
//    return; 
  } 
//  Serial.println("Berhasil koneksi ke Server"); 
  String LinkRelay; 
  HTTPClient httpRelay; 
  LinkRelay = url; 
  httpRelay.begin(LinkRelay); 
  httpRelay.GET(); 
  String responseRelay = httpRelay.getString(); 
  Serial.println(responseRelay); 
  httpRelay.end(); 
  delay(10); 
  while (client.available()) { 
    char c = client.read(); 
    responseRelay += c; 
  } 
 
  Serial.println(responseRelay); 
 
  if (responseRelay.indexOf("Nilai : 0") != -1) { 
    digitalWrite(pinrelay, LOW); 
  } else if (responseRelay.indexOf("Nilai : 1") != -1) { 
    digitalWrite(pinrelay, HIGH); 
  } 
}
