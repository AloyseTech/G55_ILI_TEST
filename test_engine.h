#include "test_ili9341.h"

#define DISPLAY_WIDTH         320
#define DISPLAY_HEIGHT        240

#define DISPLAY_COLOR_BITS    16
#define PALETTE_SIZE          16
#define PALETTE_COLOR_BITS    4
#define PALETTE_COLOR_MASK    0x0F

#define COLOR_PER_BYTE        (8/PALETTE_COLOR_BITS)
#define FRAME_BUFFER_WIDTH    (DISPLAY_WIDTH/COLOR_PER_BYTE)
#define FRAME_BUFFER_HEIGHT   (DISPLAY_HEIGHT)
#define PALETTE_ALPHA 0x0000

const uint16_t palette[PALETTE_SIZE] =
{
  PALETTE_ALPHA,
  0x001F,
  0xF800,
  0x07E0,
  0x07FF,
  0xF81F,
  0xFFE0,
  0xFB20,
  0xFFFF,
  0x7403,
  0xFBEA,
  0x75ad,
  0x624F,
  0xF81F,
  0xF0F0,
  0x0F0F
};

static uint8_t frameBuffer[FRAME_BUFFER_HEIGHT][FRAME_BUFFER_WIDTH] = {0x00};
uint16_t lineBuffer[DISPLAY_WIDTH] /*__attribute__((section("tcm")))*/;
uint8_t spiBuffer[DISPLAY_WIDTH * (DISPLAY_COLOR_BITS / 8)] /*__attribute__((section("tcm")))*/;

void engineHAL_initScreenTransfer()
{
  writeCommand(ILI9341_CASET); // Column addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite(((DISPLAY_WIDTH-1) >> 8) & 0xFF);
  spiWrite((DISPLAY_WIDTH-1) & 0xFF);

  writeCommand(ILI9341_PASET); // Row addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite(((DISPLAY_HEIGHT-1) >> 8) & 0xFF);
  spiWrite((DISPLAY_HEIGHT-1) & 0xFF);

  writeCommand(ILI9341_RAMWR); // write to RAM
}

void engineHAL_lineTransfer(uint8_t *buffer, uint32_t lineWidthInByte)
{
  pdc_tx_init((Pdc *) & (SPI5->SPI_RPR), (uint32_t)buffer, lineWidthInByte);

  pdc_enable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_TXTEN);
}

void engineHAL_waitLineTransferDone()
{
  while (!(SPI5->SPI_SR & SPI_SR_TXBUFE));
  pdc_disable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
}

void engineHAL_endScreenTransfer()
{
  //none
}


void engine_update()
{
  while (!(SPI5->SPI_SR & SPI_SR_TXBUFE));
  pdc_disable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
  engineHAL_initScreenTransfer();
  for (int line = 0; line < DISPLAY_HEIGHT; line++)
  {
    for (int col = 0; col < DISPLAY_WIDTH; col++)
    {
      uint8_t paletteColor;
      uint8_t paletteColorMask;
      paletteColor = ((frameBuffer[line][col>>1] >> ((col % COLOR_PER_BYTE) * PALETTE_COLOR_BITS)) & PALETTE_COLOR_MASK);
      uint16_t displayColor = palette[paletteColor];
      if (displayColor != PALETTE_ALPHA);
      lineBuffer[col] = displayColor;
    }
    //engineHAL_waitLineTransferDone();
    while (!(SPI5->SPI_SR & SPI_SR_TXBUFE));
    pdc_disable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);

    for (int i = 0; i < DISPLAY_WIDTH; i++)
    {
      spiBuffer[2 * i] = lineBuffer[i] >> 8 & 0xFF;
      spiBuffer[2 * i + 1] = lineBuffer[i] & 0xFF;
    }
    //spi_dmaWrite(spiBuffer, 480);
    //engineHAL_lineTransfer(spiBuffer, DISPLAY_WIDTH * (DISPLAY_COLOR_BITS / 8));
    pdc_tx_init((Pdc *) & (SPI5->SPI_RPR), (uint32_t)spiBuffer, DISPLAY_WIDTH * (DISPLAY_COLOR_BITS / 8));
    pdc_enable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_TXTEN); //PERIPH_PTCR_RXTEN
  }
  //engineHAL_endScreenTransfer();
}

