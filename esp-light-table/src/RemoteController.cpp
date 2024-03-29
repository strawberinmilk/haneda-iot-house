// Copyright [2021] <rin;>

#include <Arduino.h>
#include "RemoteController.h"

const uint16_t kIrLed = 13;
const uint16_t kRecvPin = 19;

int irDataLengthGlobal = 13;

struct {
  String name;
  decode_type_t decode_type;
  int value;
  int bits;
  } irDataGlobal[13] {
  {"lvnightlamp", decode_type_t(NEC), static_cast<int>(0x41B63DC2), static_cast<int>(32)},  // NOLINT
  {"lvon", decode_type_t(NEC), static_cast<int>(0x41B6659A), static_cast<int>(32)},  // NOLINT
  {"lvoff", decode_type_t(NEC), static_cast<int>(0x41B67D82), static_cast<int>(32)},  // NOLINT
  {"lvup", decode_type_t(NEC), static_cast<int>(0x41B65DA2), static_cast<int>(32)},  // NOLINT
  {"lvdown", decode_type_t(NEC), static_cast<int>(0x41B6DD22), static_cast<int>(32)},  // NOLINT
  {"lvsleep", decode_type_t(NEC), static_cast<int>(0x41B6F50A), static_cast<int>(32)},  // NOLINT
  {"lvlike", decode_type_t(NEC), static_cast<int>(0x41B645BA), static_cast<int>(32)},  // NOLINT
  {"fcon", decode_type_t(NEC), static_cast<int>(0xF7C03F), static_cast<int>(32)},  // NOLINT
  {"fcoff", decode_type_t(NEC), static_cast<int>(0xF740BF), static_cast<int>(32)},  // NOLINT
  {"fcwhite", decode_type_t(NEC), static_cast<int>(0xF7E01F), static_cast<int>(32)},  // NOLINT
  {"fcred"  , decode_type_t(NEC), static_cast<int>(0xF720DF), static_cast<int>(32)},  // NOLINT
  {"fcgreen", decode_type_t(NEC), static_cast<int>(0xF7A05F), static_cast<int>(32)},  // NOLINT
  {"fcblue" , decode_type_t(NEC), static_cast<int>(0xF7609F), static_cast<int>(32)},  // NOLINT
};

const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;
#define LEGACY_TIMING_INFO false
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);

decode_results results;
IRsend irsend(kIrLed);

RemoteController::RemoteController(void) {
  Serial.println("なんでこれ動かないの?");
}

void RemoteController::init(void) {
  Serial.println("RemoteController init");
  assert(irutils::lowLevelSanityCheck() == 0);
  #if DECODE_HASH
  ::irrecv.setUnknownThreshold(kMinUnknownSize);
  #endif
  ::irrecv.setTolerance(kTolerancePercentage);
  ::irrecv.enableIRIn();
  ::irsend.begin();
  irDataLength = ::irDataLengthGlobal;
  for (int i = 0; i < irDataLength; i++) {
    irData[i].name = ::irDataGlobal[i].name;
    irData[i].decode_type = ::irDataGlobal[i].decode_type;
    irData[i].value = ::irDataGlobal[i].value;
    irData[i].bits = ::irDataGlobal[i].bits;
  }
}

void RemoteController::irSend(int num) {
  Serial.println("irSend");
  Serial.println(irData[num].value);
  ::irsend.send(irData[num].decode_type, irData[num].value, irData[num].bits);
}

void RemoteController::irSendStr(String str) {
  Serial.println("irSendStr " + str);
  for (int i = 0; i < irDataLength; i++) {
    if (str == irData[i].name) {
      irSend(i);
      break;
    }
  }
}

void RemoteController::sendDebug() {
  for (int i = 9; i < 13; i++) {
    Serial.println(i);
    irSend(i);
    delay(1000);
  }
}

void RemoteController::signalGet() {
  if (::irrecv.decode(&results)) {
    Serial.print(resultToHumanReadableBasic(&results));

    // digitalWrite(14, 1);
    delay(3000);
    // digitalWrite(14, 0);
    delay(50);

    Serial.println("send");
    ::irsend.send(results.decode_type, results.value, results.bits);
  }
}
