#include "uart_frame.h"

UartFrameStatus uart_frame_init(UartFrameCollector *collector,
                                unsigned char delimiter)
{
    if (collector == 0)
    {
        return UART_FRAME_INVALID_ARGUMENT;
    }
    collector->length = 0U;
    collector->delimiter = delimiter;
    collector->discarding = 0U;
    return UART_FRAME_NONE;
}

UartFrameStatus uart_frame_note_loss(UartFrameCollector *collector)
{
    if (collector == 0)
    {
        return UART_FRAME_INVALID_ARGUMENT;
    }
    collector->length = 0U;
    collector->discarding = 1U;
    return UART_FRAME_NONE;
}

UartFrameStatus uart_frame_feed(UartFrameCollector *collector,
                                unsigned char value,
                                const unsigned char **frame,
                                unsigned char *length)
{
    if (collector == 0 || frame == 0 || length == 0)
    {
        return UART_FRAME_INVALID_ARGUMENT;
    }
    *frame = 0;
    *length = 0U;

    if (collector->discarding != 0U)
    {
        if (value == collector->delimiter)
        {
            collector->discarding = 0U;
            return UART_FRAME_DROPPED;
        }
        return UART_FRAME_NONE;
    }
    if (value == collector->delimiter)
    {
        *frame = collector->data;
        *length = collector->length;
        collector->length = 0U;
        return UART_FRAME_READY;
    }
    if (collector->length >= UART_FRAME_CAPACITY)
    {
        collector->length = 0U;
        collector->discarding = 1U;
        return UART_FRAME_NONE;
    }
    collector->data[collector->length] = value;
    ++collector->length;
    return UART_FRAME_NONE;
}
