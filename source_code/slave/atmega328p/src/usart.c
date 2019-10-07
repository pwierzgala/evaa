#include "usart.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// USART Handle
ISR(USART_RX_vect)
{
  //Read UDR0 buffer
  if(usart_recv_clbk)
    usart_recv_clbk(usart_receive());
}

void usart_int_enable()
{
  UCSR0B |= (1 << TXEN0);                      // Enable Transmiter
  UCSR0B |= (1 << RXCIE0) | (1 << RXEN0);      // Enable Receiver INT and Enable Receiver
}

void usart_int_disable()
{
  UCSR0B &= ~(1 << RXEN0);                     // Disable Receiver
  UCSR0B &= ~(1 << RXCIE0);                    // Disable Receiver INT
  UCSR0B &= ~(1 << TXEN0);                     // Disable Transmiter
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

  usart_int_enable();
}

void deinit_usart()
{
  usart_int_disable();

  UCSR0C = 0;
  UBRR0H = 0;
  UBRR0L = 0;

  usart_recv_clbk = NULL;
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

void usart_send_str(char *data)
{
  if(!data)
    return;

  char unsigned *data_ptr = (char unsigned *)data;
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

void set_usart_clbk(usart_recv_clbk_t callback)
{
  usart_recv_clbk = callback;
}
