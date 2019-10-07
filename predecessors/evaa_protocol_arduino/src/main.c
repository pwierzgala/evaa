#include <avr/interrupt.h>             // for cli() and sei() and ISR

#include "usart.h"
#include "frame_receiver.h"

#ifdef FRAME_RECEIVER_TEST
#define F_CPU 16000000UL
#include <util/delay.h>
#endif // FRAME_RECEIVER_TEST

rx_buffer_t rx_buffer;

void usart_rx_callback(char unsigned arg);

int main(void)
{
#ifdef FRAME_RECEIVER_TEST
    uint16_t counter = 0;
    evaa_protocol_frame_t frame_buffer;
#endif // FRAME_RECEIVER_TEST

    cli();
    init_rx_buffer(&rx_buffer);
    init_usart();
    set_usart_clbk(usart_rx_callback);
    sei();

    while(1) {
#ifdef FRAME_RECEIVER_TEST
    counter++;
    usart_transmit(0x0C); // to clear terminal window
    print_rx_buffer(&rx_buffer);
    if(0 == counter % 5) {
        read_frame_from_rx_buffer(&rx_buffer, &frame_buffer);
    }
    _delay_ms(1000);
#endif // FRAME_RECEIVER_TEST
    }
}

void usart_rx_callback(char unsigned arg)
{
    (void)arg;

    write_byte_to_rx_buffer(&rx_buffer, usart_receive());
}
