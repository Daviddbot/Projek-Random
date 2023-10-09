/*New blynk app project
 * https://srituhobby.com
 */
 
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_AUTH_TOKEN "X9f3bJGWIyH3GurbsiLFVvPExHdablmS" //Enter your blynk auth token

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Why";          //Enter your WIFI name
char pass[] = "150713222014"; //Enter your WIFI password


BLYNK_WRITE(V0) {
  digitalWrite(23, param.asInt());
}

void setup() {
  pinMode(23, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}
