void pdc_enable_transfer(Pdc *p_pdc, uint32_t ul_controls)
{
  p_pdc->PERIPH_PTCR = ul_controls & (PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);
}

void pdc_disable_transfer(Pdc *p_pdc, uint32_t ul_controls)
{
  p_pdc->PERIPH_PTCR = ul_controls & (PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
}

void pdc_tx_init(Pdc *p_pdc, uint32_t ul_addr, uint32_t ul_size)
{
  p_pdc->PERIPH_TPR = ul_addr;
  p_pdc->PERIPH_TCR = ul_size;
}

void pdc_rx_init(Pdc *p_pdc, uint32_t ul_addr, uint32_t ul_size)
{
  p_pdc->PERIPH_RPR = ul_addr;
  p_pdc->PERIPH_RCR = ul_size;
}

