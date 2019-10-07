#include "../inc/usart.h"
#include "../inc/utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// USART Handle
ISR(USART_RX_vect)
{
}

void init_usart()
{
  int unsigned boud = BRR;
  UBRR0H = (char unsigned)(boud >> 8);         // Boud rate 9600
  UBRR0L = (char unsigned)boud;

  UCSR0C |=  (1 << UCSZ01)  | (1 << UCSZ00);   // 8bit
  UCSR0C &= ~(1 << UPM01)   | ~(1 << UPM00);   // No Parity
  UCSR0C &= ~(1 << USBS0);                     // 1 Stop bit
  UCSR0C &= ~(1 << UMSEL01) | ~(1 << UMSEL00); // Asynchronous USART
  UCSR0C &= ~(1 << UCPOL0);                    // In Synchronous transmision - not used

  UCSR0B |= (1 << RXCIE0) | (1 << RXEN0);      // Enable Receiver INT and Enable Receiver
  UCSR0B |= (1 << TXEN0);                      // Enable Transmiter
}

void deinit_usart()
{
  UCSR0B &= ~(1 << TXEN0);                     // Disable Transmiter
  UCSR0B &= ~(1 << RXCIE0);                    // Disable Receiver INT
  UCSR0B &= ~(1 << RXEN0);                     // Disable Receiver

  UCSR0C = 0;
  UBRR0H = 0;
  UBRR0L = 0;

}

char unsigned usart_receive(void)
{
  while (!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void usart_transmit(char unsigned data)
{
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}

void usart_send_str(char unsigned *data)
{
  if(!data)
    return;

  char unsigned *data_ptr = data;
  while(*data_ptr)
  {
    usart_transmit(*data_ptr++);
  }
}

void usart_send_data(char unsigned *data, int size)
{
  if(!data)
    return;

  char unsigned *data_ptr = data;
  while(size--)
  {
    usart_transmit(*data_ptr++);
  }
}


