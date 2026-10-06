#ifndef STC8_UART_H
#define STC8_UART_H

#include "ring_buffer.h"

typedef enum
{
    STC8_UART_OK = 0,
    STC8_UART_NOT_INITIALIZED,
    STC8_UART_NOT_ATTACHED,
    STC8_UART_INVALID_ARGUMENT,
    STC8_UART_RX_OVERFLOW,
    STC8_UART_RX_BUFFER_ERROR
} Stc8UartStatus;

typedef enum
{
    STC8_UART1_ROUTE_P30_P31 = 0,
    STC8_UART1_ROUTE_P36_P37,
    STC8_UART1_ROUTE_P16_P17,
    STC8_UART1_ROUTE_P43_P44
} Stc8Uart1PinRoute;

typedef struct
{
    unsigned long system_clock_hz;
    unsigned long baud_rate;
    Stc8Uart1PinRoute pin_route;
} Stc8Uart1Config;

/* Attach and detach only while the UART1 RX producer is stopped. */
Stc8UartStatus stc8_uart_rx_attach_buffer(RingBuffer *buffer);
Stc8UartStatus stc8_uart_rx_detach_buffer(void);
unsigned char stc8_uart_rx_is_attached(void);

/* Host-testable O(1) byte path called by the real UART1 RX ISR. */
Stc8UartStatus stc8_uart_rx_handle_byte_isr(unsigned char value);

/* These operations require the caller to quiesce the RX producer. */
Stc8UartStatus stc8_uart_rx_take_error_quiesced(void);
Stc8UartStatus stc8_uart_rx_reset_buffer_quiesced(void);

/* Keil C51 target functions implemented by stc8_uart_c51.c. */
Stc8UartStatus stc8_uart1_init(const Stc8Uart1Config *config);
Stc8UartStatus stc8_uart1_start(void);
void stc8_uart1_stop(void);
Stc8UartStatus stc8_uart1_take_error(void);
Stc8UartStatus stc8_uart1_reset_rx_buffer(void);

#endif
