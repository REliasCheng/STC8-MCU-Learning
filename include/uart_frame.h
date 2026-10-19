#ifndef UART_FRAME_H
#define UART_FRAME_H

#include "ring_buffer.h"

#define UART_FRAME_CAPACITY 64U

typedef enum
{
    UART_FRAME_NONE = 0,
    UART_FRAME_READY,
    UART_FRAME_DROPPED,
    UART_FRAME_INVALID_ARGUMENT,
    UART_FRAME_BUFFER_ERROR
} UartFrameStatus;

typedef struct
{
    unsigned char data[UART_FRAME_CAPACITY];
    unsigned char length;
    unsigned char delimiter;
    unsigned char discarding;
} UartFrameCollector;

/* Main-loop only. No UART registers, interrupts, protocol decoding, or heap. */
UartFrameStatus uart_frame_init(UartFrameCollector *collector,
                                unsigned char delimiter);

/* Call after a transport loss; reject the partial frame through its delimiter. */
UartFrameStatus uart_frame_note_loss(UartFrameCollector *collector);

/* A READY frame points to internal storage until the next feed or init call. */
UartFrameStatus uart_frame_feed(UartFrameCollector *collector,
                                unsigned char value,
                                const unsigned char **frame,
                                unsigned char *length);

/*
 * Caller must keep the RX producer stopped from taking its error latch through
 * this entire call. transport_loss is a previously captured nonzero loss flag.
 * A loss discards bytes until the next delimiter; one frame/event is returned
 * per call. The caller owns producer pause/resume and any hardware timing.
 */
UartFrameStatus uart_frame_drain_quiesced(UartFrameCollector *collector,
                                          RingBuffer *rx,
                                          unsigned char transport_loss,
                                          const unsigned char **frame,
                                          unsigned char *length);

#endif
