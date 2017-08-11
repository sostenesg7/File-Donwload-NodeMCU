#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#define DBG_OUTPUT_PORT Serial

const char *ssid = "SSOUZA";
const char *password = "1234512345";

ESP8266WebServer server(80);

void handleRoot(){
  String page = "<html><body><form method=\"GET\" action=\"http://192.168.0.12/download\"><input type=\"submit\" value=\"Download\"></form></body></html>";
  server.send(200,"text/html", page);
}

void handleFileRead(){
  String dir = "/home/fila.txt"; //arquivo que quer que seja baixado
  if (SPIFFS.exists(dir))//Substitua pelo metodo de verificacao de existencia de arquivo no SD
  {
    File file = SPIFFS.open(dir, "r");//Abra um aquivo usando o SD
    server.sendHeader("Content-Disposition", "attachment; filename=\"arquivo.txt\"");
    server.streamFile(file,"text/plain");
    file.close();
  }
}

void setup(void)
{
  Serial.begin(115200);
  SPIFFS.begin();
  Serial.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid))
  {
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }
  DBG_OUTPUT_PORT.print("\nConnected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  server.on("/download", handleFileRead);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}
