#define BLYNK_TEMPLATE_ID "TMPL6uIOlM-ji"
#define BLYNK_TEMPLATE_NAME "monitoring suhu ruangan"
#define BLYNK_AUTH_TOKEN "7vYT9J51wJMvRWsVfWjVs19qjcrc4oHr"

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Masukkan informasi WiFi Anda
char auth[] = "your token";
char ssid[] = "ssid";
char pass[] = "pass";

#define LM75_ADDRESS 0x48 // Alamat I2C LM75, sesuaikan dengan alamat sensor Anda
// Definisikan pin relay
#define RELAY_PIN 18  // Ubah sesuai dengan pin yang digunakan untuk mengendalikan relay


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  Wire.begin();

  // Tidak ada inisialisasi khusus yang diperlukan untuk LM75
  
  delay(500); // Beri waktu untuk sensor inisialisasi
}

void loop() {
  Blynk.run();

  // Baca suhu dari sensor LM75
  float temperature = readLM75Temperature();

  // Kirim data suhu ke Blynk
  Blynk.virtualWrite(V0, temperature);

  //tampilkan data ke serial monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Delay beberapa detik sebelum membaca suhu lagi
  delay(1000); // Ubah sesuai kebutuhan Anda
}

float readLM75Temperature() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00); // Register kontrol LM75
  Wire.endTransmission();

  Wire.requestFrom(LM75_ADDRESS, 2);

  if (Wire.available() >= 2) {
    int rawTemp = (Wire.read() << 8) | Wire.read();
    float temperature = (rawTemp >> 7) * 0.5;
    return temperature;
  }

 // return -1.0; // Mengembalikan nilai negatif jika ada masalah pembacaan suhu
}

// Fungsi untuk mengendalikan relay berdasarkan nilai yang diterima dari Blynk
BLYNK_WRITE(V2) {
  int relayValue = param.asInt();  // Mendapatkan nilai dari tombol Blynk
  if (relayValue == 1) {
    digitalWrite(RELAY_PIN, HIGH);  // Menghidupkan relay jika nilai adalah 1
    Serial.println("relay diaktifkan");
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Mematikan relay jika nilai adalah 0
    Serial.println("relay dimatikan");
  }
}
