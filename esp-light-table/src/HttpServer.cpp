// Copyright [2021] <rin;>

#include <Arduino.h>
#include "WebServer.h"
#include "WiFi.h"
#include "HttpServer.h"

HttpServer::HttpServer(void) {
  Serial.println("なんでこれ動かないの?2");
}

WebServer server(80);

HttpServer::AccessReport HttpServer::resDecode() {
  String method = "UNKNOWN";
  switch (server.method()) {
    case HTTP_GET:
      method = "GET";
      break;
    case HTTP_POST:
      method = "POST";
      break;
    case HTTP_PUT:
      method = "PUT";
      break;
    case HTTP_DELETE:
      method = "DELETE";
      break;
    case HTTP_PATCH:
      method = "PATCH";
      break;
  }

  Arg argsTmp[server.args()];  // NOLINT
  for (uint8_t i = 0; i < server.args(); i++) {
    argsTmp[i] = Arg {server.argName(i), server.arg(i)};
  }

  AccessReport accessReport{
    server.uri(), method, server.args(), argsTmp,
  };
  Serial.println("---server access---");
  Serial.println(accessReport.uri);
  Serial.println(accessReport.method);
  Serial.println(accessReport.argsLength);
  Serial.println("---server access end---");
  Serial.println("");

  return accessReport;
}

void HttpServer::notfoundResponse() {
  resDecode();
  server.send(404, "text/plain", "404 not found");
}

void HttpServer::printMac() {
  byte byteMac[6];
  WiFi.macAddress(byteMac);
  char charMac[20];
  sprintf(charMac, "%02X:%02X:%02X:%02X:%02X:%02X:",  // NOLINT
  charMac[0],
  charMac[1],
  charMac[2],
  charMac[3],
  charMac[4],
  charMac[5]);
  Serial.println(charMac);
  return;
}

void HttpServer::init(char* ssid, char* pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("---WiFi connected---");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected: ");
  Serial.println(ssid);
  Serial.print("IPaddress: ");
  Serial.println(WiFi.localIP());
  server.onNotFound(notfoundResponse);
  Serial.println("---Wifi connected end---");
  server.begin();
}

void HttpServer::httpLoop() {
  server.handleClient();
}

void HttpServer::addTask(String uri, void func()) {
  server.on(uri, func);
}

void HttpServer::send(int status, String type, String message) {
  server.send(status, type, message);
}

