#include "core_private.h"
#include "test_pdc.h"

bool spi_init(int SPI_MODE, uint8_t divisor)
{

  pinPeripheral(SCK_ARD_PIN, GPIO_PERIPH_A);
  pinPeripheral(MISO_ARD_PIN, GPIO_PERIPH_A);
  pinPeripheral(MOSI_ARD_PIN, GPIO_PERIPH_A);
  pinPeripheral(CS_ARD_PIN, GPIO_PERIPH_A);


  if (divisor < 1)
    return false;

  //clocking the SPI peripheral
  PMC->PMC_PCR = PMC_PCR_PID(ID_FLEXCOM5) | PMC_PCR_CMD | PMC_PCR_DIV(0) | PMC_PCR_EN; //Set FLEXCOM5 peripheral clock to MCK/10 = 12MHz
  PMC->PMC_PCER0 = (1 << ID_FLEXCOM5); //enable ADC peripheral clock

  //configure flexcom peripheral opmode to 2 (SPI mode)
  FLEXCOM5->FLEXCOM_MR = FLEXCOM_MR_OPMODE(FLEXCOM_MR_OPMODE_SPI);


  //software reset SPI (why twice?)
  SPI5->SPI_CR = SPI_CR_SWRST;
  SPI5->SPI_CR = SPI_CR_SWRST;

  //configure SPI peripheral
  SPI5->SPI_MR = SPI_MR_MSTR | SPI_MR_MODFDIS | SPI_MR_PCS(0); //SPI_MR_LLB is for loop back (self test)

  //set mode (clock and phase)
  switch (SPI_MODE)
  {
    case 0:
      SPI5->SPI_CSR[0] = SPI_CSR_NCPHA;
      break;
    case 1:
      SPI5->SPI_CSR[0] = 0;
      break;
    case 2:
      SPI5->SPI_CSR[0] = SPI_CSR_CPOL | SPI_CSR_NCPHA;
      break;
    case 3:
      SPI5->SPI_CSR[0] = SPI_CSR_CPOL;
      break;
    default: //mode 0
      SPI5->SPI_CSR[0] = SPI_CSR_NCPHA;
      break;
  }

  //set clock speed (divisor) and bit per transfer (default to 8)
  SPI5->SPI_CSR[0] |= SPI_CSR_SCBR(divisor) | SPI_CSR_BITS_8_BIT;

  //enable SPI
  SPI5->SPI_CR = SPI_CR_SPIEN;

  pdc_disable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);

  return true;
}

uint8_t spi_transfer(uint8_t data_to_send)
{
  uint8_t data_received;
  //clear rx buffer?
  if (SPI5->SPI_SR & SPI_SR_RDRF)
  {
    data_received = SPI5->SPI_RDR;
    data_received = 0xFF;
  }
  //check tx ready?
  while (!(SPI5->SPI_SR & SPI_SR_TDRE))
  {
    //not ready yet...
  }

  //fill tx buffer
  SPI5->SPI_TDR = SPI_TDR_TD(data_to_send);

  //wait for rx data ready
  while (!(SPI5->SPI_SR & SPI_SR_RDRF))
  {
    //not done yet...
  }

  data_received = SPI5->SPI_RDR & 0x00000000FF;

  return data_received;
}

void spi_write(uint8_t data_to_send)
{
  //is tx buf ready?
  while (!(SPI5->SPI_SR & SPI_SR_TDRE));

  //fill tx buffer once ready
  SPI5->SPI_TDR = SPI_TDR_TD(data_to_send);
}

uint8_t spi_read()
{
  return spi_transfer(0xFF);
}

void spi_waitFinish()
{
  //wait for rx data ready
  while (!(SPI5->SPI_SR & SPI_SR_RDRF))
  {
    //not done yet...
  }
}

void spi_dmaWrite(void *ul_addr_out, uint32_t ul_size_out)
{
  pdc_tx_init((Pdc *) & (SPI5->SPI_RPR), (uint32_t)ul_addr_out, ul_size_out);

  pdc_enable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_TXTEN); //PERIPH_PTCR_RXTEN
  //wait until done
  while (!(SPI5->SPI_SR & SPI_SR_TXBUFE));
  //delay(10);
  //disable pdc
  pdc_disable_transfer((Pdc *) & (SPI5->SPI_RPR), PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
}

