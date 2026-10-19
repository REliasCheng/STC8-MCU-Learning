#include "uart_frame.h"

UartFrameStatus uart_frame_drain_quiesced(UartFrameCollector *collector,
                                          RingBuffer *rx,
                                          unsigned char transport_loss,
                                          const unsigned char **frame,
                                          unsigned char *length)
{
    unsigned char overflowed;
    unsigned char value;
    RingBufferStatus pop_status;
    UartFrameStatus frame_status;

    if (collector == 0 || rx == 0 || frame == 0 || length == 0)
    {
        return UART_FRAME_INVALID_ARGUMENT;
    }
    *frame = 0;
    *length = 0U;

    if (ring_buffer_take_overflow(rx, &overflowed) != RING_BUFFER_OK)
    {
        (void)uart_frame_note_loss(collector);
        return UART_FRAME_BUFFER_ERROR;
    }
    if (transport_loss != 0U || overflowed != 0U)
    {
        (void)uart_frame_note_loss(collector);
    }

    for (;;)
    {
        pop_status = ring_buffer_pop(rx, &value);
        if (pop_status == RING_BUFFER_EMPTY)
        {
            return UART_FRAME_NONE;
        }
        if (pop_status != RING_BUFFER_OK)
        {
            (void)uart_frame_note_loss(collector);
            return UART_FRAME_BUFFER_ERROR;
        }
        frame_status = uart_frame_feed(collector, value, frame, length);
        if (frame_status != UART_FRAME_NONE)
        {
            return frame_status;
        }
    }
}
