#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef enum
{
    RING_BUFFER_OK = 0,
    RING_BUFFER_EMPTY,
    RING_BUFFER_FULL,
    RING_BUFFER_INVALID_ARGUMENT
} RingBufferStatus;

typedef struct
{
    unsigned char *storage;
    unsigned char capacity;
    volatile unsigned char head;
    volatile unsigned char tail;
    volatile unsigned char overflow;
} RingBuffer;

RingBufferStatus ring_buffer_init(
    RingBuffer *buffer,
    unsigned char *storage,
    unsigned char capacity);

/*
 * Reset changes producer-owned and consumer-owned state. The caller must
 * ensure that the producer is quiesced, for example by disabling the UART RX
 * interrupt, before calling this function.
 */
RingBufferStatus ring_buffer_reset(RingBuffer *buffer);

/* Single-producer entry point. Only the producer may update head. */
RingBufferStatus ring_buffer_push_isr(
    RingBuffer *buffer,
    unsigned char value);

/* Single-consumer entry point. Only the consumer may update tail. */
RingBufferStatus ring_buffer_pop(
    RingBuffer *buffer,
    unsigned char *value);

RingBufferStatus ring_buffer_available(
    const RingBuffer *buffer,
    unsigned char *available);

RingBufferStatus ring_buffer_is_empty(
    const RingBuffer *buffer,
    unsigned char *is_empty);

RingBufferStatus ring_buffer_is_full(
    const RingBuffer *buffer,
    unsigned char *is_full);

/*
 * This read-and-clear operation is not atomic with ring_buffer_push_isr().
 * The caller must quiesce the producer for the complete call so that a new
 * overflow event cannot be lost between the read and the clear.
 */
RingBufferStatus ring_buffer_take_overflow(
    RingBuffer *buffer,
    unsigned char *overflowed);

#endif
