//Display pin definition
#define CS_PORT_PIN     PIO_PA11A_SPI5_NPCS0
#define MISO_PORT_PIN   PIO_PA12A_SPI5_MISO
#define MOSI_PORT_PIN   PIO_PA13A_SPI5_MOSI
#define SCK_PORT_PIN    PIO_PA14A_SPI5_SPCK
#define DC_PORT_PIN     PIO_PA26
#define RST_PORT_PIN    PIO_PA23
#define LED_PORT_PIN    PIO_PA25

#define CS_ARD_PIN     33
#define MISO_ARD_PIN   31
#define MOSI_ARD_PIN   30
#define SCK_ARD_PIN    32
#define DC_ARD_PIN     8
#define RST_ARD_PIN    11
#define LED_ARD_PIN    9

//#include "test_spi.h"
//#include "test_ili9341.h"
#include "test_gfx.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  spi_init(0, 1); //mode 0, clock divisor 1 (120MHz ?????)


  /*
    analogReadResolution(12);
    pinMode(11, OUTPUT);
    pinMode(10, OUTPUT);
  */

  pinMode(LED_ARD_PIN, OUTPUT);
  digitalWrite(LED_ARD_PIN, HIGH);

  pinMode(DC_ARD_PIN, OUTPUT);
  digitalWrite(DC_ARD_PIN, LOW);
  delay(100);
  digitalWrite(DC_ARD_PIN, HIGH);
  delay(100);
  digitalWrite(DC_ARD_PIN, LOW);
  delay(100);

  pinMode(RST_ARD_PIN, OUTPUT);


  display_init();
  Serial.println("Done");


  uint32_t t = micros();
  for (int i = 0; i < 100; i++)
  {
    display_fill(random(0xFFFF));
  }
  t = micros() - t;
  Serial.println("Standard SPI (100 frames) :");
  Serial.print(t);
  Serial.println(" us");
  Serial.print(1000000.0 / t * 100.0);
  Serial.println(" fps");

  delay(1000);

  t = micros();
  for (int i = 0; i < 100; i++)
  {
    display_dmaFill(random(0xFFFF));
  }
  t = micros() - t;
  Serial.println("SPI with DMA (100 frames) :");
  Serial.print(t);
  Serial.println(" us");
  Serial.print(1000000.0 / t * 100.0);
  Serial.println(" fps");
}

bool state = 1;
uint32_t timer = 0; uint32_t fpsTimer = 0;
uint8_t c = 0;
void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(7));
  if (millis() - timer > 20)
  {
    if (c == 30)
    {
      c = 0;
      Serial.print(30000.0 / (millis() - fpsTimer));
      Serial.println("fps");
      fpsTimer = millis();
    }
    timer = millis();
    engine_update();
    //memset(frameBuffer, random(0xFF), FRAME_BUFFER_HEIGHT * FRAME_BUFFER_WIDTH);
    c++;
  }
  drawPixel(random(320), random(240), random(0xF));
}
