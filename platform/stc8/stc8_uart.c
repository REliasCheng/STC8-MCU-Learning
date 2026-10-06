#include "stc8_uart.h"

static RingBuffer *stc8_uart_rx_buffer = 0;
static volatile unsigned char stc8_uart_rx_error =
    (unsigned char)STC8_UART_OK;

Stc8UartStatus stc8_uart_rx_attach_buffer(RingBuffer *buffer)
{
    unsigned char available;

    if (buffer == 0)
    {
        return STC8_UART_INVALID_ARGUMENT;
    }

    if (ring_buffer_available(buffer, &available) != RING_BUFFER_OK)
    {
        return STC8_UART_INVALID_ARGUMENT;
    }

    stc8_uart_rx_buffer = buffer;
    stc8_uart_rx_error = (unsigned char)STC8_UART_OK;
    return STC8_UART_OK;
}

Stc8UartStatus stc8_uart_rx_detach_buffer(void)
{
    stc8_uart_rx_buffer = 0;
    stc8_uart_rx_error = (unsigned char)STC8_UART_OK;
    return STC8_UART_OK;
}

unsigned char stc8_uart_rx_is_attached(void)
{
    return (unsigned char)(stc8_uart_rx_buffer != 0);
}

Stc8UartStatus stc8_uart_rx_handle_byte_isr(unsigned char value)
{
    RingBufferStatus status;

    if (stc8_uart_rx_buffer == 0)
    {
        stc8_uart_rx_error = (unsigned char)STC8_UART_NOT_ATTACHED;
        return STC8_UART_NOT_ATTACHED;
    }

    status = ring_buffer_push_isr(stc8_uart_rx_buffer, value);
    if (status == RING_BUFFER_OK)
    {
        return STC8_UART_OK;
    }

    if (status == RING_BUFFER_FULL)
    {
        stc8_uart_rx_error = (unsigned char)STC8_UART_RX_OVERFLOW;
        return STC8_UART_RX_OVERFLOW;
    }

    stc8_uart_rx_error = (unsigned char)STC8_UART_RX_BUFFER_ERROR;
    return STC8_UART_RX_BUFFER_ERROR;
}

Stc8UartStatus stc8_uart_rx_take_error_quiesced(void)
{
    Stc8UartStatus status;

    status = (Stc8UartStatus)stc8_uart_rx_error;
    stc8_uart_rx_error = (unsigned char)STC8_UART_OK;
    return status;
}

Stc8UartStatus stc8_uart_rx_reset_buffer_quiesced(void)
{
    if (stc8_uart_rx_buffer == 0)
    {
        return STC8_UART_NOT_ATTACHED;
    }

    if (ring_buffer_reset(stc8_uart_rx_buffer) != RING_BUFFER_OK)
    {
        stc8_uart_rx_error = (unsigned char)STC8_UART_RX_BUFFER_ERROR;
        return STC8_UART_RX_BUFFER_ERROR;
    }

    stc8_uart_rx_error = (unsigned char)STC8_UART_OK;
    return STC8_UART_OK;
}
