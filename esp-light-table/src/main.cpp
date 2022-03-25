// Copyright [2021] <rin;>

#include <Arduino.h>
#include "RemoteController.h"
#include "HttpServer.h"

RemoteController remoteController = RemoteController();
HttpServer httpServer = HttpServer();

String html = R"(
      <!DOCTYPE html>
      <html lang="ja">
      <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Document</title>
      </head>
      <body>
        <div class="button"><a href=/ir/lvnightlamp>常夜灯</a></div>
        <div class="button"><a href=/ir/lvon>ON</a></div>
        <div class="button"><a href=/ir/lvdown>OFF</a></div>
        <div class="button"><a href=/ir/lvup>明るさup</a></div>
        <div class="button"><a href=/ir/lvoff>明るさdown</a></div>
        <div class="button"><a href=/ir/lvsleep>おやすみ</a></div>
        <div class="button"><a href=/ir/lvlike>お好み</a></div>
        <p></p>
        <div class="button"><a href=/desk/on>デスクオン</a></div>
        <div class="button"><a href=/desk/off>デスクオフ</a></div>
      </body>
      </html>
      )";

void httpIrTask(String str) {
  Serial.println("TASK");
  Serial.println(str);
}

int setUpI = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("boot");
  pinMode(14, OUTPUT);

  remoteController.init();

  String strSsid = mySSID;
  String strPass = myPASS;
  char charSsid[strSsid.length()+1];  // NOLINT
  char charPass[strPass.length()+1];  // NOLINT
  strSsid.toCharArray(charSsid, strSsid.length()+1);
  strPass.toCharArray(charPass, strPass.length()+1);
  httpServer.init(charSsid, charPass);
  httpServer.addTask("/", [](){
    httpServer.resDecode();
    httpServer.send(200, "text/html; charset=utf-8", html);
  });
  for (setUpI = 0; setUpI < remoteController.irDataLength; setUpI++) {
    httpServer.addTask("/ir/" + remoteController.irData[setUpI].name, [](){
      HttpServer::AccessReport res = httpServer.resDecode();
      httpServer.send(200, "text/html; charset=utf-8", html);
      String command = res.uri;
      command.replace("/ir/", "");
      remoteController.irSendStr(command);
    });
  }

  httpServer.addTask("/desk/on", [](){
    HttpServer::AccessReport res = httpServer.resDecode();
    httpServer.send(200, "text/html; charset=utf-8", html);
    digitalWrite(14, true);
  });
  httpServer.addTask("/desk/off", [](){
    HttpServer::AccessReport res = httpServer.resDecode();
    httpServer.send(200, "text/html; charset=utf-8", html);
    digitalWrite(14, false);
  });
}

void loop() {
  httpServer.httpLoop();
}
