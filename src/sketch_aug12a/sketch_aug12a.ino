//#define F_CPU 16000000UL // Defining the CPU Frequency

#include <avr/io.h>      // Contains all the I/O Register Macros
#include <util/delay.h>  // Generates a Blocking Delay

#define USART_BAUDRATE 2400 // Desired Baud Rate
#define BAUD_PRESCALER (((16000000UL / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0<<UMSEL00) // USART Mode Selection

#define DISABLED    (0<<UPM00)
#define EVEN_PARITY (2<<UPM00)
#define ODD_PARITY  (3<<UPM00)
#define PARITY_MODE  DISABLED // USART Parity Bit Selection

#define ONE_BIT (0<<USBS0)
#define TWO_BIT (1<<USBS0)
#define STOP_BIT ONE_BIT      // USART Stop Bit Selection

#define FIVE_BIT  (0<<UCSZ00)
#define SIX_BIT   (1<<UCSZ00)
#define SEVEN_BIT (2<<UCSZ00)
#define EIGHT_BIT (3<<UCSZ00)
#define DATA_BIT   EIGHT_BIT  // USART Data Bit Selection
#define EEPROM_SIZE 2047

enum{
  EEPROM_OK,
  EEPROM_WRITE_FAIL,
  EEPROM_INVALID_ADDR 
};

void USART_Init()
{
  // Set Baud Rate
  UBRR0H = BAUD_PRESCALER >> 8;
  UBRR0L = BAUD_PRESCALER;
  
  // Set Frame Format
  UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;
  
  // Enable Receiver and Transmitter
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void USART_TransmitPolling(uint8_t DataByte)
{
  while (( UCSR0A & (1<<UDRE0)) == 0) {}; // Do nothing until UDR is ready
  UDR0 = DataByte;
}

uint8_t USART_ReceivePolling()
{
  uint8_t DataByte;
  while (( UCSR0A & (1<<RXC0)) == 0) {}; // Do nothing until data have been received
  DataByte = UDR0 ;
//  USART_TransmitPolling(DataByte);
  return DataByte;
}


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

int main()
{ 
  uint8_t localData,data1;
  uint16_t i=0;
  uint16_t j;
  USART_Init();
  while (1)
  {

    localData = USART_ReceivePolling();
    EEPROM_write(i, localData);
    EEPROM_read(i,data1);
    USART_TransmitPolling(data1);
    i += 1;
//    if (localData == '*')
//    {
//      for (j=0; j<=i; j++)
//      {
//        EEPROM_read(j,data1);
//        USART_TransmitPolling(data1);
//      }
//    }
    
  }

  return 0;
}
