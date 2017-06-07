#include "test_engine.h"

void drawPixel(int x, int y, uint8_t palColor)
{
  palColor = palColor & 0xF;
  uint8_t temp = frameBuffer[x][y / 2];
  if (y % 2)
  {
    palColor <<= 4;
    palColor &= 0xF0;
  }
  else
  {
    palColor &= 0xF;
  }
  temp |= palColor;
  frameBuffer[x][y / 2] = temp;
}

