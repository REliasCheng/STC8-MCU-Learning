#include <assert.h>
#include <stdio.h>

#include "uart_frame.h"
#include "ring_buffer.h"

static void test_two_frames_and_empty_frame(void)
{
    UartFrameCollector collector;
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'O', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'K', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_READY);
    assert(length == 2U && frame[0] == (unsigned char)'O' && frame[1] == (unsigned char)'K');
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_READY);
    assert(length == 0U);
    assert(uart_frame_feed(&collector, (unsigned char)'Y', &frame, &length) == UART_FRAME_NONE);
    assert(frame == 0 && length == 0U);
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'Y');
}

static void test_configured_delimiter_and_binary_zero(void)
{
    UartFrameCollector collector;
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(uart_frame_init(&collector, 0x7EU) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, 0x00U, &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, 0xFFU, &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, 0x7EU, &frame, &length) == UART_FRAME_READY);
    assert(length == 2U && frame[0] == 0x00U && frame[1] == 0xFFU);
}

static void test_exact_capacity_and_overlong_recovery(void)
{
    UartFrameCollector collector;
    const unsigned char *frame = 0;
    unsigned char length = 0U;
    unsigned int index;

    assert(uart_frame_init(&collector, 0xFEU) == UART_FRAME_NONE);
    for (index = 0U; index < UART_FRAME_CAPACITY; ++index) {
        assert(uart_frame_feed(&collector, (unsigned char)index, &frame, &length) == UART_FRAME_NONE);
    }
    assert(uart_frame_feed(&collector, 0xFEU, &frame, &length) == UART_FRAME_READY);
    assert(length == UART_FRAME_CAPACITY);
    for (index = 0U; index < UART_FRAME_CAPACITY; ++index) {
        assert(frame[index] == (unsigned char)index);
    }

    for (index = 0U; index <= UART_FRAME_CAPACITY; ++index) {
        assert(uart_frame_feed(&collector, (unsigned char)'x', &frame, &length) == UART_FRAME_NONE);
    }
    assert(uart_frame_feed(&collector, 0xFEU, &frame, &length) == UART_FRAME_DROPPED);
    assert(frame == 0 && length == 0U);
    assert(uart_frame_feed(&collector, (unsigned char)'a', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, 0xFEU, &frame, &length) == UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'a');
}

static void test_transport_loss_discards_partial_frame(void)
{
    UartFrameCollector collector;
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'o', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_note_loss(&collector) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'k', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_DROPPED);
    assert(frame == 0 && length == 0U);
    assert(uart_frame_feed(&collector, (unsigned char)'n', &frame, &length) == UART_FRAME_NONE);
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'n');
}

static void test_invalid_arguments_do_not_consume_byte(void)
{
    UartFrameCollector collector;
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(uart_frame_init(0, (unsigned char)'\n') == UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_note_loss(0) == UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(uart_frame_feed(0, (unsigned char)'x', &frame, &length) == UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_feed(&collector, (unsigned char)'x', 0, &length) == UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_feed(&collector, (unsigned char)'x', &frame, 0) == UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_feed(&collector, (unsigned char)'\n', &frame, &length) == UART_FRAME_READY);
    assert(length == 0U);
}

static void test_existing_ring_buffer_drains_into_collector(void)
{
    RingBuffer rx;
    UartFrameCollector collector;
    unsigned char storage[5];
    unsigned char value = 0U;
    unsigned char length = 0U;
    const unsigned char *frame = 0;
    UartFrameStatus status = UART_FRAME_NONE;

    assert(ring_buffer_init(&rx, storage, 5U) == RING_BUFFER_OK);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'O') == RING_BUFFER_OK);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'K') == RING_BUFFER_OK);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'\n') == RING_BUFFER_OK);
    while (ring_buffer_pop(&rx, &value) == RING_BUFFER_OK) {
        status = uart_frame_feed(&collector, value, &frame, &length);
    }
    assert(status == UART_FRAME_READY);
    assert(length == 2U && frame[0] == (unsigned char)'O' && frame[1] == (unsigned char)'K');
}

int main(void)
{
    test_two_frames_and_empty_frame();
    test_configured_delimiter_and_binary_zero();
    test_exact_capacity_and_overlong_recovery();
    test_transport_loss_discards_partial_frame();
    test_invalid_arguments_do_not_consume_byte();
    test_existing_ring_buffer_drains_into_collector();
    puts("UART frame collector tests passed (6 cases)");
    return 0;
}
