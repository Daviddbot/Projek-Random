#include <WiFi.h>
#include <HTTPClient.h>

#define SENSOR_GETAR_PIN 35 // Pin yang digunakan untuk sensor getar
#define SENSOR_SUARA_PIN 34 // Pin yang digunakan untuk sensor suara


char ssid[] = "";
char pass[] = "";  


const char* serverName ="https://";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi"); 
  delay(500); 
  // Beri waktu untuk sensor inisialisasi

  pinMode(SENSOR_GETAR_PIN, INPUT);
  pinMode(SENSOR_SUARA_PIN, INPUT);
}

void loop() {
  // Baca nilai dari sensor getar dan konversi ke rentang 0-100
  
  int nilai_getar = analogRead(SENSOR_GETAR_PIN);
  int getar = map(nilai_getar, 0, 4095, 0, 100);
  
  // Baca nilai dari sensor suara dan konversi ke rentang 0-100
  
  int nilai_suara = analogRead(SENSOR_SUARA_PIN);
  int suara = map(nilai_suara, 0, 4095, 0, 100);

  // Tampilkan nilai sensor getar dan sensor suara

  
  Serial.print("Nilai Sensor Getar: ");
  Serial.println(getar);
  Serial.print("Nilai Sensor Suara: ");
  Serial.println(suara);

    String strsr = String(suara);
    String strgr = String(getar);
    
    HTTPClient http;
    http.begin(serverName);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "suara=" + strsr + "&getar=" + strgr;
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode>0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();  
  delay(1000); 
  // Tunggu 1 detik sebelum membaca ulang
}
