#include <stdio.h>

#include "ring_buffer.h"
#include "stc8_uart.h"

static const char *current_test_name = "";

static int report_failure(
    const char *file,
    int line,
    const char *expression,
    int expected,
    int actual)
{
    (void)printf(
        "FAIL %s: %s:%d: %s (expected %d, actual %d)\n",
        current_test_name,
        file,
        line,
        expression,
        expected,
        actual);
    return 1;
}

#define CHECK_EQUAL(expected, actual)                                      \
    do                                                                     \
    {                                                                      \
        int expected_value = (int)(expected);                              \
        int actual_value = (int)(actual);                                  \
        if (expected_value != actual_value)                                \
        {                                                                  \
            return report_failure(                                         \
                __FILE__,                                                  \
                __LINE__,                                                  \
                #actual,                                                   \
                expected_value,                                            \
                actual_value);                                             \
        }                                                                  \
    } while (0)

static int test_attach_valid_buffer(void)
{
    RingBuffer buffer;
    unsigned char storage[4];

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(1, stc8_uart_rx_is_attached());
    return 0;
}

static int test_attach_rejects_null_buffer(void)
{
    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(
        STC8_UART_INVALID_ARGUMENT,
        stc8_uart_rx_attach_buffer(0));
    CHECK_EQUAL(0, stc8_uart_rx_is_attached());
    return 0;
}

static int test_attach_rejects_invalid_buffer(void)
{
    RingBuffer buffer;

    buffer.storage = 0;
    buffer.capacity = 0U;
    buffer.head = 0U;
    buffer.tail = 0U;
    buffer.overflow = 0U;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(
        STC8_UART_INVALID_ARGUMENT,
        stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(0, stc8_uart_rx_is_attached());
    return 0;
}

static int test_received_byte_reaches_ring_buffer(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(0x5AU));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(0x5A, value);
    return 0;
}

static int test_adapter_preserves_fifo_order(void)
{
    RingBuffer buffer;
    unsigned char storage[5];
    unsigned char value;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 5U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(10U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(20U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(30U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(10, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(20, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(30, value);
    return 0;
}

static int test_full_buffer_propagates_overflow(void)
{
    RingBuffer buffer;
    unsigned char storage[3];
    unsigned char value;
    unsigned char overflowed;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 3U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(1U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(2U));
    CHECK_EQUAL(
        STC8_UART_RX_OVERFLOW,
        stc8_uart_rx_handle_byte_isr(3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(1, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(2, value);
    CHECK_EQUAL(RING_BUFFER_EMPTY, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_take_overflow(&buffer, &overflowed));
    CHECK_EQUAL(1, overflowed);
    return 0;
}

static int test_adapter_error_latch_clears(void)
{
    RingBuffer buffer;
    unsigned char storage[2];

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 2U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(1U));
    CHECK_EQUAL(
        STC8_UART_RX_OVERFLOW,
        stc8_uart_rx_handle_byte_isr(2U));
    CHECK_EQUAL(
        STC8_UART_RX_OVERFLOW,
        stc8_uart_rx_take_error_quiesced());
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_take_error_quiesced());
    return 0;
}

static int test_unattached_receive_is_safe(void)
{
    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(
        STC8_UART_NOT_ATTACHED,
        stc8_uart_rx_handle_byte_isr(0xA5U));
    CHECK_EQUAL(
        STC8_UART_NOT_ATTACHED,
        stc8_uart_rx_take_error_quiesced());
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_take_error_quiesced());
    return 0;
}

static int test_detach_stops_forwarding(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char available;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_detach_buffer());
    CHECK_EQUAL(0, stc8_uart_rx_is_attached());
    CHECK_EQUAL(
        STC8_UART_NOT_ATTACHED,
        stc8_uart_rx_handle_byte_isr(1U));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_available(&buffer, &available));
    CHECK_EQUAL(0, available);
    return 0;
}

static int test_quiesced_reset_clears_buffer_and_error(void)
{
    RingBuffer buffer;
    unsigned char storage[3];
    unsigned char empty;

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 3U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(1U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_handle_byte_isr(2U));
    CHECK_EQUAL(
        STC8_UART_RX_OVERFLOW,
        stc8_uart_rx_handle_byte_isr(3U));
    CHECK_EQUAL(
        STC8_UART_OK,
        stc8_uart_rx_reset_buffer_quiesced());
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_is_empty(&buffer, &empty));
    CHECK_EQUAL(1, empty);
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_take_error_quiesced());
    return 0;
}

static int test_reset_rejects_unattached_state(void)
{
    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(
        STC8_UART_NOT_ATTACHED,
        stc8_uart_rx_reset_buffer_quiesced());
    return 0;
}

static int test_attach_clears_previous_unattached_error(void)
{
    RingBuffer buffer;
    unsigned char storage[4];

    (void)stc8_uart_rx_detach_buffer();
    CHECK_EQUAL(
        STC8_UART_NOT_ATTACHED,
        stc8_uart_rx_handle_byte_isr(1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_attach_buffer(&buffer));
    CHECK_EQUAL(STC8_UART_OK, stc8_uart_rx_take_error_quiesced());
    return 0;
}

typedef int (*TestFunction)(void);

typedef struct
{
    const char *name;
    TestFunction function;
} TestCase;

int main(void)
{
    static const TestCase tests[] = {
        {"attach_valid", test_attach_valid_buffer},
        {"attach_null", test_attach_rejects_null_buffer},
        {"attach_invalid", test_attach_rejects_invalid_buffer},
        {"byte_forwarding", test_received_byte_reaches_ring_buffer},
        {"adapter_fifo", test_adapter_preserves_fifo_order},
        {"full_propagation", test_full_buffer_propagates_overflow},
        {"error_latch", test_adapter_error_latch_clears},
        {"unattached_receive", test_unattached_receive_is_safe},
        {"detach", test_detach_stops_forwarding},
        {"quiesced_reset", test_quiesced_reset_clears_buffer_and_error},
        {"reset_unattached", test_reset_rejects_unattached_state},
        {"attach_clears_error", test_attach_clears_previous_unattached_error}
    };
    unsigned int index;
    unsigned int failed;
    unsigned int count;

    failed = 0U;
    count = (unsigned int)(sizeof(tests) / sizeof(tests[0]));
    for (index = 0U; index < count; ++index)
    {
        current_test_name = tests[index].name;
        if (tests[index].function() != 0)
        {
            ++failed;
        }
        else
        {
            (void)printf("PASS %s\n", current_test_name);
        }
    }

    (void)printf(
        "SUMMARY total=%u passed=%u failed=%u\n",
        count,
        count - failed,
        failed);

    return (failed == 0U) ? 0 : 1;
}
