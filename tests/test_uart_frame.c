#include <assert.h>
#include <stdio.h>

#include "uart_frame.h"
#include "ring_buffer.h"
#include "stc8_uart.h"

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

static void test_quiesced_drain_preserves_frame_boundaries(void)
{
    RingBuffer rx;
    UartFrameCollector collector;
    unsigned char storage[8];
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(ring_buffer_init(&rx, storage, 8U) == RING_BUFFER_OK);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(stc8_uart_rx_attach_buffer(&rx) == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'A') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'\n') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'B') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'\n') == STC8_UART_OK);
    /* In this host test the simulated producer is now stopped. */
    assert(stc8_uart_rx_take_error_quiesced() == STC8_UART_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'A');
    assert(stc8_uart_rx_take_error_quiesced() == STC8_UART_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'B');
    assert(stc8_uart_rx_take_error_quiesced() == STC8_UART_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_NONE);
    assert(frame == 0 && length == 0U);
    assert(stc8_uart_rx_detach_buffer() == STC8_UART_OK);
}

static void test_adapter_loss_blocks_truncated_frame(void)
{
    RingBuffer rx;
    UartFrameCollector collector;
    unsigned char storage[3];
    const unsigned char *frame = 0;
    unsigned char length = 0U;
    Stc8UartStatus error;

    assert(ring_buffer_init(&rx, storage, 3U) == RING_BUFFER_OK);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(stc8_uart_rx_attach_buffer(&rx) == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'A') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'B') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'C') == STC8_UART_RX_OVERFLOW);
    error = stc8_uart_rx_take_error_quiesced();
    assert(error == STC8_UART_RX_OVERFLOW);
    assert(uart_frame_drain_quiesced(&collector, &rx, (unsigned char)(error != STC8_UART_OK),
                                     &frame, &length) == UART_FRAME_NONE);
    assert(frame == 0 && length == 0U);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'\n') == STC8_UART_OK);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'N') == STC8_UART_OK);
    assert(stc8_uart_rx_take_error_quiesced() == STC8_UART_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_DROPPED);
    assert(frame == 0 && length == 0U);
    assert(stc8_uart_rx_handle_byte_isr((unsigned char)'\n') == STC8_UART_OK);
    assert(stc8_uart_rx_take_error_quiesced() == STC8_UART_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_READY);
    assert(length == 1U && frame[0] == (unsigned char)'N');
    assert(stc8_uart_rx_detach_buffer() == STC8_UART_OK);
}

static void test_ring_overflow_flag_is_enough_to_block_frame(void)
{
    RingBuffer rx;
    UartFrameCollector collector;
    unsigned char storage[3];
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(ring_buffer_init(&rx, storage, 3U) == RING_BUFFER_OK);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'A') == RING_BUFFER_OK);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'B') == RING_BUFFER_OK);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'C') == RING_BUFFER_FULL);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_NONE);
    assert(frame == 0 && length == 0U);
    assert(rx.overflow == 0U);
    assert(ring_buffer_push_isr(&rx, (unsigned char)'\n') == RING_BUFFER_OK);
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_DROPPED);
}

static void test_quiesced_drain_rejects_bad_queue(void)
{
    RingBuffer rx;
    UartFrameCollector collector;
    unsigned char storage[3];
    const unsigned char *frame = 0;
    unsigned char length = 0U;

    assert(ring_buffer_init(&rx, storage, 3U) == RING_BUFFER_OK);
    assert(uart_frame_init(&collector, (unsigned char)'\n') == UART_FRAME_NONE);
    assert(uart_frame_drain_quiesced(0, &rx, 0U, &frame, &length) ==
           UART_FRAME_INVALID_ARGUMENT);
    assert(uart_frame_drain_quiesced(&collector, 0, 0U, &frame, &length) ==
           UART_FRAME_INVALID_ARGUMENT);
    rx.head = rx.capacity;
    assert(uart_frame_drain_quiesced(&collector, &rx, 0U, &frame, &length) ==
           UART_FRAME_BUFFER_ERROR);
    assert(frame == 0 && length == 0U);
    assert(collector.discarding != 0U);
}

int main(void)
{
    test_two_frames_and_empty_frame();
    test_configured_delimiter_and_binary_zero();
    test_exact_capacity_and_overlong_recovery();
    test_transport_loss_discards_partial_frame();
    test_invalid_arguments_do_not_consume_byte();
    test_existing_ring_buffer_drains_into_collector();
    test_quiesced_drain_preserves_frame_boundaries();
    test_adapter_loss_blocks_truncated_frame();
    test_ring_overflow_flag_is_enough_to_block_frame();
    test_quiesced_drain_rejects_bad_queue();
    puts("UART frame collector tests passed (10 cases)");
    return 0;
}
