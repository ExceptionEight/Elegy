#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

void setup()
{
  pinMode (2, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin("MGTS_GPON_E879", "77S33FGN");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/api", api);
  server.begin();
}

void api () {
  Serial.println("api called");
  if (server.arg("power") == "1") {
    digitalWrite (2, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite (2, LOW);
    Serial.println("0");
  }
}

void loop() {
  server.handleClient();
}