#include "ring_buffer.h"

static unsigned char ring_buffer_valid(const RingBuffer *buffer)
{
    if (buffer == 0)
    {
        return 0U;
    }

    if ((buffer->storage == 0) || (buffer->capacity < 2U))
    {
        return 0U;
    }

    return 1U;
}

static unsigned char ring_buffer_next_index(
    const RingBuffer *buffer,
    unsigned char index)
{
    index = (unsigned char)(index + 1U);
    if (index >= buffer->capacity)
    {
        index = 0U;
    }

    return index;
}

RingBufferStatus ring_buffer_init(
    RingBuffer *buffer,
    unsigned char *storage,
    unsigned char capacity)
{
    if ((buffer == 0) || (storage == 0) || (capacity < 2U))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    buffer->storage = storage;
    buffer->capacity = capacity;
    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->overflow = 0U;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_reset(RingBuffer *buffer)
{
    if (ring_buffer_valid(buffer) == 0U)
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->overflow = 0U;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_push_isr(
    RingBuffer *buffer,
    unsigned char value)
{
    unsigned char next_head;

    if (ring_buffer_valid(buffer) == 0U)
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    next_head = ring_buffer_next_index(buffer, buffer->head);
    if (next_head == buffer->tail)
    {
        buffer->overflow = 1U;
        return RING_BUFFER_FULL;
    }

    buffer->storage[buffer->head] = value;
    buffer->head = next_head;

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_pop(
    RingBuffer *buffer,
    unsigned char *value)
{
    unsigned char current_tail;

    if ((ring_buffer_valid(buffer) == 0U) || (value == 0))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    current_tail = buffer->tail;
    if (current_tail == buffer->head)
    {
        return RING_BUFFER_EMPTY;
    }

    *value = buffer->storage[current_tail];
    buffer->tail = ring_buffer_next_index(buffer, current_tail);

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_available(
    const RingBuffer *buffer,
    unsigned char *available)
{
    unsigned char head;
    unsigned char tail;

    if ((ring_buffer_valid(buffer) == 0U) || (available == 0))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    head = buffer->head;
    tail = buffer->tail;

    if (head >= tail)
    {
        *available = (unsigned char)(head - tail);
    }
    else
    {
        *available = (unsigned char)(buffer->capacity - tail + head);
    }

    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_is_empty(
    const RingBuffer *buffer,
    unsigned char *is_empty)
{
    if ((ring_buffer_valid(buffer) == 0U) || (is_empty == 0))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    *is_empty = (unsigned char)(buffer->head == buffer->tail);
    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_is_full(
    const RingBuffer *buffer,
    unsigned char *is_full)
{
    if ((ring_buffer_valid(buffer) == 0U) || (is_full == 0))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    *is_full = (unsigned char)(
        ring_buffer_next_index(buffer, buffer->head) == buffer->tail);
    return RING_BUFFER_OK;
}

RingBufferStatus ring_buffer_take_overflow(
    RingBuffer *buffer,
    unsigned char *overflowed)
{
    if ((ring_buffer_valid(buffer) == 0U) || (overflowed == 0))
    {
        return RING_BUFFER_INVALID_ARGUMENT;
    }

    *overflowed = buffer->overflow;
    buffer->overflow = 0U;
    return RING_BUFFER_OK;
}
