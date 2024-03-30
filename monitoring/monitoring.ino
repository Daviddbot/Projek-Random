#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include "MQ7.h"
#include "DHT.h"

#define DHTPIN 23    // Digital pin connected to the DHT sensor
const int sensorPin = 34; // Pin sensor hujan terhubung
int sensorValue = 0;      // Variabel untuk menyimpan nilai pembacaan sensor

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
MQ7 mq7(35, 5.0);
DHT dht(DHTPIN, DHTTYPE);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  int hujanLevel;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200; // UTC+7 (7 * 3600 seconds)
const int   daylightOffset_sec = 0;  // Jakarta tidak menerapkan Daylight Saving Time


char ssid[] = "wifi";
char pass[] = "wifi";  
const char* serverName ="https://";


void setup() {
  Serial.begin(115200); /* Baud rate untuk komunikasi serial */
  // Inisialisasi WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//  printLocalTime();
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  postdata();
  delay(2000);
}
void postdata()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }

    char formattedTime[50];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println(String() + "WAKTU dari NTP : " + formattedTime);
    Serial.println();
    
  suhu();
  mq77();
  hujan();
    
    String strkel = String(dht.readHumidity());
    String strsuhu = String(dht.readTemperature());
    String strpolusi = String(mq7.getPPM());
    String strhujan = String(hujanLevel);    
    String strwaktu = String(formattedTime);
    
//  Posting Data ke Database Server dengan Protokol HTTP
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "suhu=" + strsuhu + "&kelembaban=" + strkel + "&polusi=" + strpolusi + "&tanggal=" + strwaktu ;
    
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
    delay(5000);

  }

void hujan() {
  // Baca nilai sensor
  sensorValue = analogRead(sensorPin);

  // Konversi nilai sensor menjadi rentang 0-100
  int hujanLevel = map(sensorValue, 0, 4095, 0, 100);

  // Tampilkan nilai sensor di Serial Monitor
  Serial.print("Nilai Sensor Hujan: ");
  Serial.println(hujanLevel);

  delay(1000); // Tunda selama 1 detik sebelum membaca sensor lagi
}
void mq77()
{
  Serial.print("Karbon Monoksida : ");
  Serial.println(mq7.getPPM());
}

void suhu(){
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F")); 
  }
