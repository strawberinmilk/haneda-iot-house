// Copyright [2021] <rin;>

#ifndef ESP_LIGHT_TABLE_SRC_HTTPSERVER_H_
#define ESP_LIGHT_TABLE_SRC_HTTPSERVER_H_
#include "WebServer.h"
#include "WiFi.h"




class HttpServer {
  public:
    HttpServer(void);
    typedef struct {
      String argName;
      String arg;
    } Arg;

    typedef struct {
      String uri;
      String method;
      int argsLength;
      Arg* args[10];
    } AccessReport;

    static AccessReport resDecode(void);
    static void printMac(void);
    static void init(char*, char*);
    static void addTask(String, void());
    static void httpLoop(void);
    static void send(int, String, String);
  private:
    static void notfoundResponse(void);
};

#endif  // ESP_LIGHT_TABLE_SRC_HTTPSERVER_H_
