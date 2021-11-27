// Copyright [2021] <rin;>

#include <Arduino.h>
#include "RemoteController.h"

RemoteController remoteController = RemoteController();

void setup() {
  Serial.begin(115200);
  Serial.println("boot");
  remoteController.init();
}

void loop() {
  // Serial.println("loop");
  // remoteController.debugLoop();
  remoteController.debug();
}
