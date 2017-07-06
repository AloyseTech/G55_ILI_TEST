#include "test_engine.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void drawPixel(int x, int y, uint8_t palColor)
{
  palColor = palColor & 0xF;
  if (palColor != PALETTE_ALPHA)
  {
    uint8_t temp = frameBuffer[y][x / 2];
    if (x % 2)
    {
      palColor <<= 4;
      palColor &= 0xF0;
      temp &= 0x0F;
    }
    else
    {
      palColor &= 0xF;
      temp &= 0xF0;
    }
    temp |= palColor;
    frameBuffer[y][x / 2] = temp;
  }
}

void drawLine(int x0, int y0, int x1, int y1, uint8_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}


void memcpy_with_bitshift(unsigned char *dest, const uint8_t *src, size_t n, int bit)
{
  int i;

  memcpy(dest, src, n);

  for (i = 0; i < n; i++) {
    dest[i] >> bit;
  }

  for (i = 0; i < n; i++) {
    dest[i+1] |= (src[i] << (8 - bit));
  }
}

void drawSprite(int x, int y, const uint8_t*sprite, int w, int h)
{
  //  Serial.println("==============");
  //  Serial.println("x = " + String(x) + "\ty = " + String(y) + "\tw = " + String(w) + "\th = " + String(h));
  //w = min(w, 240 - x);
  //h = min(h, 320 - y);

  //x=0;
  //y=0;

  if (1)//w<2 | h<2)
  {
    for (int line = 0; line < h; line++)
      for (int col = 0; col < w; col++)
      {
        drawPixel(x + col, y + line, ((col % 2) ? sprite[line * (w / 2) + (col / 2)] : sprite[line * (w / 2) + (col / 2)] >> 4));
      }
  }
  else if (x % 2)
  {
    for (int line = 0; line < h; line++)
    {
      //drawPixel(x, y + line, sprite[line * (w >> 1)]);
      //drawPixel(x + (w - 1), y + line, sprite[line * (w >> 1) + ((w - 1) >> 1)]);
      //memcpy(&frameBuffer[y + line][x], sprite + (line * (w >> 1)), (w >> 1));
      memcpy_with_bitshift(&frameBuffer[y + line][x], sprite + (line * (w >> 1)), (w >> 1),4);
    }
  }
  else
  {
    for (int line = 0; line < h; line++)
    {
      memcpy(&frameBuffer[y + line][x], sprite + (line * (w >> 1)), (w >> 1));
    }
  }
}

