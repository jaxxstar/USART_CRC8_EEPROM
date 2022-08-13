#include <string.h>
#include <avr/io.h>


#define EEPROM_SIZE 1023


enum{
  EEPROM_OK,
  EEPROM_WRITE_FAIL,
  EEPROM_INVALID_ADDR 
};

uint8_t EEPROM_read(uint16_t uiAddress, uint8_t *data)
{
  
  if(uiAddress > EEPROM_SIZE){
    return EEPROM_INVALID_ADDR;
  }
  
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE));
  /* Set up address register */
  EEARH = (uiAddress & 0xFF00) >> 8;
  EEARL = (uiAddress & 0x00FF);
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from Data Register */
  *data = EEDR;
  return EEPROM_OK;
}

uint8_t EEPROM_write(uint16_t uiAddress, uint8_t ucData)
{
  if(uiAddress > EEPROM_SIZE){
    return EEPROM_INVALID_ADDR;
  }
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE));
  /* Set up address and Data Registers */
  EEARH = (uiAddress & 0xFF00) >> 8;
  EEARL = (uiAddress & 0x00FF);
  EEDR = ucData;
  /* Write logical one to EEMPE */
  EECR |= (1<<EEMPE);
  /* Start eeprom write by setting EEPE */
  EECR |= (1<<EEPE);
  return EEPROM_OK;
}
int main(){
  while(1){}
}
