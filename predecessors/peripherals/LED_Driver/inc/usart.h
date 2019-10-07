#ifndef INC_USART_H_
#define INC_USART_H_

#define BAUD 9600
#define BRR (F_CPU / 16 / BAUD - 1)


void init_usart();
void deinit_usart();

char unsigned usart_receive(void);

void usart_transmit(char unsigned data);
void usart_send_str(char unsigned *data);
void usart_send_data(char unsigned *data, int size);




#endif /* INC_USART_H_ */
