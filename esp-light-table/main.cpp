#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

const uint16_t kRecvPin = 19;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;

#define LEGACY_TIMING_INFO false
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;
#include <IRsend.h>
int irLEDpin = 18;
const uint16_t kIrLed = irLEDpin;
IRsend irsend(kIrLed);

  enum livingLight {
    nightLamp = 0,
    on,
    down,
    up,
    off,
    sleepTimer,
    like = 6,
  };

  enum fcLightBulb {
    fcOn = 7,
    fcoff,
  };

  typedef struct {
  String name;
  decode_type_t decode_type;
  int value;
  int bits;
  } Irdata;

  const Irdata irData[] {
    {"nightLamp",decode_type_t(NEC),int(0x41B63DC2),int(32)},
    {"on",decode_type_t(NEC),int(0x41B6659A),int(32)},
    {"down",decode_type_t(NEC),int(0x41B6DD22),int(32)},
    {"up",decode_type_t(NEC),int(0x41B65DA2),int(32)},
    {"off",decode_type_t(NEC),int(0x41B67D82),int(32)},
    {"sleep",decode_type_t(NEC),int(0x41B6F50A),int(32)},
    {"like",decode_type_t(NEC),int(0x41B645BA),int(32)}
  };

/*
nightlamp
Protocol  : NEC
Code      : 0x41B63DC2 (32 Bits)
全灯
Protocol  : NEC
Code      : 0x41B6659A (32 Bits)
暗く
Protocol  : NEC
Code      : 0x41B6DD22 (32 Bits)
明るく
Protocol  : NEC
Code      : 0x41B65DA2 (32 Bits)
消灯
Protocol  : NEC
Code      : 0x41B67D82 (32 Bits)
スリープタイマー
Protocol  : NEC
Code      : 0x41B6F50A (32 Bits)
お好み
Protocol  : NEC
Code      : 0x41B645BA (32 Bits)


on
Protocol  : NEC
Code      : 0xF7C03F (32 Bits)
off
Protocol  : NEC
Code      : 0xF740BF (32 Bits)
white
Protocol  : NEC
Code      : 0xF7E01F (32 Bits)
red
Protocol  : NEC
Code      : 0xF720DF (32 Bits)
green
Protocol  : NEC
Code      : 0xF7A05F (32 Bits)
blue
Protocol  : NEC
Code      : 0xF7609F (32 Bits)


濃いオレンジ
Protocol  : NEC
Code      : 0xF710EF (32 Bits)
オレンジ
Protocol  : NEC
Code      : 0xF730CF (32 Bits)
濃い黄色
Protocol  : NEC
Code      : 0xF708F7 (32 Bits)
黄色
Protocol  : NEC
Code      : 0xF728D7 (32 Bits)
エメラルドグリーン青寄り
Protocol  : NEC
Code      : 0xF7906F (32 Bits)
薄水色
Protocol  : NEC
Code      : 0xF7B04F (32 Bits)
水色
Protocol  : NEC
Code      : 0xF78877 (32 Bits)
濃い水色
Protocol  : NEC
Code      : 0xF7A857 (32 Bits)
薄青色
Protocol  : NEC
Code      : 0xF750AF (32 Bits)
赤紫色
Protocol  : NEC
Code      : 0xF750AF (32 Bits)
青紫色
Protocol  : NEC
Code      : 0xF748B7 (32 Bits)
ピンク
Protocol  : NEC
Code      : 0xF76897 (32 Bits)
flash
Protocol  : NEC
Code      : 0xF7D02F (32 Bits)
strobe
Protocol  : NEC
Code      : 0xF7F00F (32 Bits)
fade
Protocol  : NEC
Code      : 0xF7C837 (32 Bits)
smooth
Protocol  : NEC
Code      : 0xF7C837 (32 Bits)
*/

void setup() {
  Serial.begin(115200);
  Serial.println("boot");
  assert(irutils::lowLevelSanityCheck() == 0);
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.setTolerance(kTolerancePercentage);
  irrecv.enableIRIn();
  irsend.begin();
  pinMode(14, OUTPUT);
}

void irSend(int num) {
  Serial.println(num);
  Serial.println(irData[num].value);
  
  irsend.send(irData[num].decode_type, irData[num].value, irData[num].bits);
}

void loop() {
  /*
  Serial.println(irData[0].name);
  Serial.println(irData[0].decode_type);
  irsend.send(irData[0].decode_type, irData[0].value, irData[0].bits);
  delay(1000);
  */
  for(int i=0; i<7; i++){
    irSend(i);
    delay(1000);
  }
  
}

void debugLoop() {
  if (irrecv.decode(&results)) {
    Serial.print(resultToHumanReadableBasic(&results));

    digitalWrite(14,1);
    delay(3000);
    digitalWrite(14,0);
    delay(50);
    Serial.println("send");

    irsend.send(results.decode_type, results.value, results.bits);
  }
}