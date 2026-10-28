#include "stc8h8k64u_sdcc.h"
#include "stc8_uart.h"
#include "uart_frame.h"

/* SDCC emits the UART1 vector from the translation unit containing main. */
void stc8_uart1_isr(void) __interrupt(4);

#define SYSTEM_CLOCK_HZ 24000000UL
#define UART1_BAUD_RATE 9600UL
#define RX_STORAGE_SIZE 64U
#define FRAME_DELIMITER '\n'

static __xdata RingBuffer rx_buffer;
static __xdata unsigned char rx_storage[RX_STORAGE_SIZE];
static __xdata UartFrameCollector frame_collector;
static volatile unsigned char frames_seen;
static volatile unsigned char dropped_frames;

void main(void)
{
    Stc8Uart1Config config;
    UartFrameStatus frame_status;
    Stc8UartStatus transport_status;
    const unsigned char *frame;
    unsigned char length;

    EA = 0;
    if (ring_buffer_init(&rx_buffer, rx_storage, RX_STORAGE_SIZE) !=
        RING_BUFFER_OK ||
        uart_frame_init(&frame_collector, FRAME_DELIMITER) != UART_FRAME_NONE ||
        stc8_uart_rx_attach_buffer(&rx_buffer) != STC8_UART_OK)
    {
        for (;;) {}
    }

    config.system_clock_hz = SYSTEM_CLOCK_HZ;
    config.baud_rate = UART1_BAUD_RATE;
    config.pin_route = STC8_UART1_ROUTE_P30_P31;
    if (stc8_uart1_init(&config) != STC8_UART_OK ||
        stc8_uart1_start() != STC8_UART_OK)
    {
        for (;;) {}
    }
    EA = 1;

    for (;;)
    {
        ES = 0;
        transport_status = stc8_uart_rx_take_error_quiesced();
        frame_status = uart_frame_drain_quiesced(
            &frame_collector, &rx_buffer,
            (unsigned char)(transport_status != STC8_UART_OK),
            &frame, &length);
        ES = 1;
        if (frame_status == UART_FRAME_READY)
        {
            /* Build-level observable application state; no hardware output. */
            ++frames_seen;
        }
        else if (frame_status == UART_FRAME_DROPPED ||
                 frame_status == UART_FRAME_BUFFER_ERROR)
        {
            ++dropped_frames;
        }
    }
}
