#ifndef _HEADER_USART_
#define _HEADER_USART_

#define BAUD 9600
#define BRR (F_CPU / 16 / BAUD - 1)

typedef void(*usart_recv_clbk_t)(char unsigned);
usart_recv_clbk_t usart_recv_clbk;

void init_usart();
void deinit_usart();

char unsigned usart_receive(void);

void usart_transmit(char unsigned data);
void usart_send_str(char unsigned *data);
void usart_send_data(char unsigned *data, int size);

void set_usart_clbk(usart_recv_clbk_t callback);

#endif
