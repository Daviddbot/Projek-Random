#include <HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define SS_PIN 21  
#define RST_PIN 22
#define pinrelay 15
#define LED_G   2
Servo servo;
MFRC522 mfrc522(SS_PIN, RST_PIN); 


String tagid; 
bool dataSent = false; // Flag to indicate if data has been sent

const char* ssid = "Why"; //Nama WiFi
const char* password = "150713222014";//Password WiFi
//ganti domain
const char* serverName ="https://esp32doorlocksystemrfid.online/simpan.php?tagid=";
const char* url ="https://esp32doorlocksystemrfid.online/update_switch.php";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200; // UTC+7 (7 * 3600 seconds)
const int   daylightOffset_sec = 0;  // Jakarta tidak menerapkan Daylight Saving Time

void setup()
{
  Serial.begin(115200); 
  SPI.begin();    
  mfrc522.PCD_Init();  
  pinMode(15, OUTPUT); 
  servo.attach(4);  //Attach servo to pin 9/
  servo.write(0);  // Move servo to initial position
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.println("Connecting..");
  }

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop()
{
    if (!mfrc522.PICC_IsNewCardPresent()) {
        Serial.print("tersambung");
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        relay();
        relay();
        relay();
        relay();
    }

    Serial.println();
        database();
        dataSent = true;
           delay(2000); 
} 

void database(){
  WiFiClient client;
  Serial.println();
  Serial.print("UID tag: ");
  String tagid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    tagid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    tagid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagid.toUpperCase();
          
  // Posting Data ke Database Server dengan Protokol HTTP
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "tagid=" + tagid ;
    
  Serial.print("Data Permintaan HTTP: ");
  Serial.println(httpRequestData);
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode > 0) {
    Serial.print("Kode Respons HTTP: ");
    Serial.println(httpResponseCode);

    relay();

  } else {
    Serial.print("Kesalahan kode: ");
    Serial.println(httpResponseCode);
  }

  http.end();

}

void openServo() {
  // Move servo to 90-degree position
  servo.write(90);
  delay(5000);
  servo.write(0); // Return servo to initial position
}

void relay() {
  // Kirim permintaan POST ke server PHP
  WiFiClient client1;
  const int httpPort = 80;
  if (!client1.connect(url, httpPort)) {
    Serial.println("Gagal Koneksi ke Server");
  }
  
  String LinkRelay;
  HTTPClient httpRelay;
  LinkRelay = url;
  httpRelay.begin(LinkRelay);
  httpRelay.GET();
  String responseRelay = httpRelay.getString();
  Serial.println(responseRelay);
  httpRelay.end();

  while (client1.available()) {
    char c = client1.read();
    responseRelay += c;
  }
  Serial.println(responseRelay);

  if (responseRelay.indexOf("Nilai : 0") != -1) {
    digitalWrite(pinrelay, LOW);
  } else if (responseRelay.indexOf("Nilai : 1") != -1) {
    digitalWrite(pinrelay, HIGH);
    int nilai = 1; // Nilai variabel diambil dari database, misalnya dari variabel yang menyimpan data dari database
    if (nilai == 1) {
      openServo(); // Panggil fungsi untuk membuka servo
    }
  }

  delay(1);
}