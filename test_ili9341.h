#include "arduino.h"
#include "ili_cmd.h"
#include "test_spi.h"

void writeCommand(uint8_t cmd)
{
  digitalWrite(DC_ARD_PIN, LOW);
  //delay(1);
  spi_write(cmd);
  spi_waitFinish();
  //delay(1);
  digitalWrite(DC_ARD_PIN, HIGH);
}

void spiWrite(uint8_t data)
{
  spi_write(data);
  //delay(1);
}


void display_dmaFill(uint16_t color)
{
  uint8_t bufout[480];

  for (int i = 0; i < 240; i++)
  {
    bufout[2 * i] = (color >> 8) & 0xFF;
    bufout[2 * i + 1] = color & 0xFF;
  }

  writeCommand(ILI9341_CASET); // Column addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite(0);
  spiWrite(239);

  writeCommand(ILI9341_PASET); // Row addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite((319 >> 8) & 0xFF);
  spiWrite(319 & 0xFF);

  writeCommand(ILI9341_RAMWR); // write to RAM

  for (int i = 0; i < 320; i++)
    spi_dmaWrite(bufout, 480);

}
void display_fill(uint16_t color)
{
  writeCommand(ILI9341_CASET); // Column addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite(0);
  spiWrite(239);

  writeCommand(ILI9341_PASET); // Row addr set
  spiWrite(0);
  spiWrite(0);
  spiWrite((319 >> 8) & 0xFF);
  spiWrite(319 & 0xFF);

  writeCommand(ILI9341_RAMWR); // write to RAM

  for (int y = 0; y < 320; y++)
  {
    for (int x = 0; x < 240; x++)
    {
      spi_write((color >> 8) & 0xFF);
      spi_write((color) & 0xFF);
    }
  }
}

int display_init()
{
  Serial.println("Display init...");

  digitalWrite(RST_ARD_PIN, HIGH);
  delay(200);
  digitalWrite(RST_ARD_PIN, LOW);
  delay(200);
  digitalWrite(RST_ARD_PIN, HIGH);
  delay(500);


  writeCommand(0xEF);
  spiWrite(0x03);
  spiWrite(0x80);
  spiWrite(0x02);

  writeCommand(0xCF);
  spiWrite(0x00);
  spiWrite(0XC1);
  spiWrite(0X30);

  writeCommand(0xED);
  spiWrite(0x64);
  spiWrite(0x03);
  spiWrite(0X12);
  spiWrite(0X81);

  writeCommand(0xE8);
  spiWrite(0x85);
  spiWrite(0x00);
  spiWrite(0x78);

  writeCommand(0xCB);
  spiWrite(0x39);
  spiWrite(0x2C);
  spiWrite(0x00);
  spiWrite(0x34);
  spiWrite(0x02);

  writeCommand(0xF7);
  spiWrite(0x20);

  writeCommand(0xEA);
  spiWrite(0x00);
  spiWrite(0x00);

  writeCommand(ILI9341_PWCTR1);    //Power control
  spiWrite(0x23);   //VRH[5:0]

  writeCommand(ILI9341_PWCTR2);    //Power control
  spiWrite(0x10);   //SAP[2:0];BT[3:0]

  writeCommand(ILI9341_VMCTR1);    //VCM control
  spiWrite(0x3e);
  spiWrite(0x28);

  writeCommand(ILI9341_VMCTR2);    //VCM control2
  spiWrite(0x86);  //--

  writeCommand(ILI9341_MADCTL);    // Memory Access Control
  spiWrite(0x48);

  writeCommand(ILI9341_VSCRSADD); // Vertical scroll
  spiWrite(0);                 // Zero
  spiWrite(0);                 // Zero

  writeCommand(ILI9341_PIXFMT);
  spiWrite(0x55);

  writeCommand(ILI9341_FRMCTR1);
  spiWrite(0x00);
  spiWrite(0x18); //default : 0x18

  writeCommand(ILI9341_DFUNCTR);    // Display Function Control
  spiWrite(0x08);
  spiWrite(0x82);
  spiWrite(0x27);

  writeCommand(0xF2);    // 3Gamma Function Disable
  spiWrite(0x00);

  writeCommand(ILI9341_GAMMASET);    //Gamma curve selected
  spiWrite(0x01);

  writeCommand(ILI9341_GMCTRP1);    //Set Gamma
  spiWrite(0x0F);
  spiWrite(0x31);
  spiWrite(0x2B);
  spiWrite(0x0C);
  spiWrite(0x0E);
  spiWrite(0x08);
  spiWrite(0x4E);
  spiWrite(0xF1);
  spiWrite(0x37);
  spiWrite(0x07);
  spiWrite(0x10);
  spiWrite(0x03);
  spiWrite(0x0E);
  spiWrite(0x09);
  spiWrite(0x00);

  writeCommand(ILI9341_GMCTRN1);    //Set Gamma
  spiWrite(0x00);
  spiWrite(0x0E);
  spiWrite(0x14);
  spiWrite(0x03);
  spiWrite(0x11);
  spiWrite(0x07);
  spiWrite(0x31);
  spiWrite(0xC1);
  spiWrite(0x48);
  spiWrite(0x08);
  spiWrite(0x0F);
  spiWrite(0x0C);
  spiWrite(0x31);
  spiWrite(0x36);
  spiWrite(0x0F);

  writeCommand(ILI9341_MADCTL);
  spiWrite(MADCTL_MV | MADCTL_BGR);

  writeCommand(ILI9341_SLPOUT);    //Exit Sleep
  delay(50);

  display_fill(0);

  writeCommand(ILI9341_DISPON);    //Display on
  delay(50);

  writeCommand(0xD9);  // woo sekret command?
  spiWrite(0x10);
  writeCommand(0x0A);
  uint8_t r = spi_read();
  return r;
}

