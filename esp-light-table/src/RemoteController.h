// Copyright [2021] <rin;>

#ifndef ESP_LIGHT_TABLE_SRC_REMOTECONTROLLER_H_
#define ESP_LIGHT_TABLE_SRC_REMOTECONTROLLER_H_

#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <IRsend.h>

class RemoteController{
  public:
    RemoteController(void);
    void init(void);
    void irSend(int);
    void sendDebug(void);
    void signalGet(void);
    void irSendStr(String);
    int irDataLength;
    struct {
      String name;
      decode_type_t decode_type;
      int value;
      int bits;
      } irData[13];

  private:
    decode_results results;
    IRrecv irrecv(uint16_t, uint16_t, uint8_t, bool, uint8_t);
    IRsend irsend(uint16_t, bool, bool);
    IRrecv setUnknownThreshold(uint16_t);
    IRrecv enableIRIn(void);
    IRsend begin(void);
    IRsend send(decode_type_t, int, int);
    IRrecv decode(decode_results);
    uint16_t kRecvPin;
    uint32_t kBaudRate;
    uint16_t kCaptureBufferSize;
    uint8_t kTimeout;
    uint16_t kMinUnknownSize;
    uint8_t kTolerancePercentage;
    uint16_t kIrLed;
};

#endif  // ESP_LIGHT_TABLE_SRC_REMOTECONTROLLER_H_
