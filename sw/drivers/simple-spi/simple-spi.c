/*
 * Simple SPI module driver
 *
 * Julius Baxter, julius.baxter@orsoc.se
 *
 */

#include "board.h"
#include "simple-spi.h"
#include "cpu-utils.h"

const int spi_base_adr[1] = {SPI0_BASE};

void 
spi_core_enable(int core)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPCR)) |= SIMPLESPI_SPCR_SPE;
}

void 
spi_core_disable(int core)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPCR)) &= ~SIMPLESPI_SPCR_SPE;
}

void 
spi_core_interrupt_enable(int core)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPCR)) |= SIMPLESPI_SPCR_SPIE;
}

void 
spi_core_interrupt_disable(int core)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPCR)) &= ~SIMPLESPI_SPCR_SPIE;
}

void 
spi_core_interrupt_flag_clear(int core)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPSR)) = SIMPLESPI_SPSR_SPIF;
}

void 
spi_core_clock_setup(int core, char polarity, char phase, char rate,
			  char ext_rate)
{
  char spcr = REG8((spi_base_adr[core] + SIMPLESPI_SPCR));

  if (polarity)
    spcr |= SIMPLESPI_SPCR_CPOL;
  else
    spcr &= ~SIMPLESPI_SPCR_CPOL;

  if (phase)
    spcr |= SIMPLESPI_SPCR_CPHA;
  else
    spcr &= ~SIMPLESPI_SPCR_CPHA;

  spcr = (spcr & ~SIMPLESPI_SPCR_SPR) | (rate & SIMPLESPI_SPCR_SPR);

  REG8((spi_base_adr[core] + SIMPLESPI_SPCR)) = spcr;

  char sper = REG8((spi_base_adr[core] + SIMPLESPI_SPER));
  
  sper = (sper & ~SIMPLESPI_SPER_ESPR) | (ext_rate & SIMPLESPI_SPER_ESPR);

  REG8((spi_base_adr[core] + SIMPLESPI_SPER)) = sper;

}

void 
spi_core_set_int_count(int core, char cnt)
{  
  char sper = REG8((spi_base_adr[core] + SIMPLESPI_SPER));
  
  sper = (sper & ~SIMPLESPI_SPER_ICNT) | cnt;
  
  REG8((spi_base_adr[core] + SIMPLESPI_SPER)) = sper;
  
}
// No decode on slave select lines, so assert correct bit to select slave
void 
spi_core_slave_select(int core, char slave_sel_dec)
{  
  REG8((spi_base_adr[core] + SIMPLESPI_SSPU)) = slave_sel_dec;
}

int 
spi_core_data_avail(int core)
{
  return !!!(REG8((spi_base_adr[core]+SIMPLESPI_SPSR))&SIMPLESPI_SPSR_RFEMPTY);
}

int 
spi_core_write_avail(int core)
{
  return !!!(REG8((spi_base_adr[core]+SIMPLESPI_SPSR))&SIMPLESPI_SPSR_WFFULL);
}

// Should call spi_core_write_avail() before calling this, we don't check
void 
spi_core_write_data(int core, char data)
{
  REG8((spi_base_adr[core] + SIMPLESPI_SPDR)) = data;
}

char 
spi_core_read_data(int core)
{
  return REG8((spi_base_adr[core] + SIMPLESPI_SPDR));
}
