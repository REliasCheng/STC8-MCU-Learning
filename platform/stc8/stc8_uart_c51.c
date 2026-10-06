#include <STC/STC8H.H>

#include "stc8_uart.h"

#define STC8_UART1_INTERRUPT_NUMBER 4
#define STC8_UART1_ROUTE_MASK 0xC0U
#define STC8_UART1_MODE1_RECEIVE 0x50U
#define STC8_AUXR_T2_RUN 0x10U
#define STC8_AUXR_T2_COUNTER 0x08U
#define STC8_AUXR_T2_1T 0x04U
#define STC8_AUXR_UART1_TIMER2 0x01U

static unsigned char stc8_uart1_initialized = 0U;

Stc8UartStatus stc8_uart1_init(const Stc8Uart1Config *config)
{
    unsigned long timer_ticks;
    unsigned long reload_value;

    if (config == 0)
    {
        return STC8_UART_INVALID_ARGUMENT;
    }

    if ((config->system_clock_hz == 0UL) ||
        (config->baud_rate == 0UL) ||
        (config->pin_route > STC8_UART1_ROUTE_P43_P44))
    {
        return STC8_UART_INVALID_ARGUMENT;
    }

    timer_ticks = config->system_clock_hz / 4UL / config->baud_rate;
    if ((timer_ticks == 0UL) || (timer_ticks > 65535UL))
    {
        return STC8_UART_INVALID_ARGUMENT;
    }

    reload_value = 65536UL - timer_ticks;
    stc8_uart1_stop();
    stc8_uart1_initialized = 0U;

    P_SW1 = (unsigned char)(
        (P_SW1 & (unsigned char)(~STC8_UART1_ROUTE_MASK)) |
        ((unsigned char)config->pin_route << 6));

    SCON = STC8_UART1_MODE1_RECEIVE;

    AUXR = (unsigned char)(AUXR &
        (unsigned char)(~(STC8_AUXR_T2_RUN | STC8_AUXR_T2_COUNTER)));
    AUXR = (unsigned char)(AUXR |
        STC8_AUXR_T2_1T | STC8_AUXR_UART1_TIMER2);

    T2H = (unsigned char)(reload_value >> 8);
    T2L = (unsigned char)reload_value;
    AUXR = (unsigned char)(AUXR | STC8_AUXR_T2_RUN);

    RI = 0;
    TI = 0;
    stc8_uart1_initialized = 1U;
    return STC8_UART_OK;
}

Stc8UartStatus stc8_uart1_start(void)
{
    if (stc8_uart1_initialized == 0U)
    {
        return STC8_UART_NOT_INITIALIZED;
    }

    if (stc8_uart_rx_is_attached() == 0U)
    {
        return STC8_UART_NOT_ATTACHED;
    }

    RI = 0;
    ES = 1;
    return STC8_UART_OK;
}

void stc8_uart1_stop(void)
{
    ES = 0;
}

Stc8UartStatus stc8_uart1_take_error(void)
{
    unsigned char interrupt_enabled;
    Stc8UartStatus status;

    interrupt_enabled = (unsigned char)ES;
    ES = 0;
    status = stc8_uart_rx_take_error_quiesced();
    ES = interrupt_enabled;
    return status;
}

Stc8UartStatus stc8_uart1_reset_rx_buffer(void)
{
    unsigned char interrupt_enabled;
    Stc8UartStatus status;

    interrupt_enabled = (unsigned char)ES;
    ES = 0;
    status = stc8_uart_rx_reset_buffer_quiesced();
    ES = interrupt_enabled;
    return status;
}

void stc8_uart1_isr(void) interrupt STC8_UART1_INTERRUPT_NUMBER
{
    unsigned char received_byte;

    if (RI != 0)
    {
        received_byte = SBUF;
        RI = 0;
        (void)stc8_uart_rx_handle_byte_isr(received_byte);
    }

    if (TI != 0)
    {
        TI = 0;
    }
}
