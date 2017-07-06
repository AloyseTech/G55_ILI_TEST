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
#include "test_sprite.h"

void setup() {
  // put your setup code here, to run once:

  //CMCC enable?
  if (!CMCC->CMCC_SR)
  {
    CMCC->CMCC_CTRL |= CMCC_CTRL_CEN | (4 << 4);
  }
  //end cmcc

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  spi_init(0, 2); //mode 0, clock divisor 2 (60MHz ?????)


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

  /*
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
  */
}

bool state = 1;
uint32_t timer = 0; uint32_t fpsTimer = 0;
uint8_t c = 0;
void loop() {
  /*
  memset(frameBuffer, 0, FRAME_BUFFER_HEIGHT * FRAME_BUFFER_WIDTH);
  for (int i = 0; i < 64; i++)
  {
    drawSprite(random(240 - 32), random(320 - 32), sprite, 32, 32);
    //drawLine(random(micros()) % 240, random(micros()) % 320, random(micros()) % 240, random(micros()) % 320, i % 16);
    //drawLine(random(micros()) % 240, random(micros()) % 320, random(micros()) % 240, random(micros()) % 320, i % 16);
    //drawLine(random(micros()) % 240, random(micros()) % 320, random(micros()) % 240, random(micros()) % 320, i % 16);
    //drawLine(random(micros()) % 240, random(micros()) % 320, random(micros()) % 240, random(micros()) % 320, i % 16);
  }
  */
  
    if (!digitalRead(7))
    {
      memset(frameBuffer, 0, FRAME_BUFFER_HEIGHT * FRAME_BUFFER_WIDTH);
      drawSprite(32,0, square32, 32, 32);
      drawSprite(31,16, square32, 32, 32);
      drawSprite(1,0, lineV1, 1, 10);
      drawSprite(2,0, lineV1, 1, 10);
      drawSprite(3,0, lineV1, 1, 10);
      engine_update();
      delay(500);
    }
  


  digitalWrite(LED_BUILTIN, digitalRead(7));
  if (millis() - timer > 0)
  {
    if (c == 10)
    {
      c = 0;
      Serial.print(10000.0 / (millis() - fpsTimer));
      Serial.println("fps");
      fpsTimer = millis();
    }
    timer = millis();
    //memset(frameBuffer, random(0xFF), FRAME_BUFFER_HEIGHT * FRAME_BUFFER_WIDTH);
    //drawPixel(16,32,0xF);
    //drawLine((0), (0), (239), (300), (0xF));

    engine_update();

    c++;
  }


}
