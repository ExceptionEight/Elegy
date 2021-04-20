#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266FtpServer.h>
ESP8266WebServer server(80);
FtpServer ftp;

void setup()
{
  pinMode (2, OUTPUT);
  Serial.begin(115200);
  WiFi.begin("MGTS_GPON_E879", "77S33FGN");
  WiFi.softAP("Elegy", "12344321");
  SPIFFS.begin();
  server.begin();
  ftp.begin("admin", "admin");

  File f = SPIFFS.open ("/wifi", "r");
  if (!f) {
    //Serial.print("AP");
    //WiFi.softAP("Elegy", "12344321");
  }
  else {
    Serial.print("Connecting");
    //WiFi.begin("MGTS_GPON_E879", "77S33FGN");
  }

  //while (WiFi.status() != WL_CONNECTED)
  //{
  //  delay(500);
  //  Serial.print(".");
  //}
  server.on("/powerSwitch", powerSwitch);
  //server.on("/setWifiConnection", api);
  server.onNotFound([](){
    if (!handleFileRead(server.uri()))
    server.send (404, "text/plain", "DURILKA");
  });
}

void setWifiConnection () {

}

void powerSwitch() {
  if (server.arg("power" == "1")) {
    digitalWrite(D4, LOW);
  }
  else {
    digitalWrite (D4, HIGH);
  }
}

void loop() {
  server.handleClient();
  ftp.handleFTP();
}

bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if(SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".svg")) return "image/svg";
  return "text/plain";
}
