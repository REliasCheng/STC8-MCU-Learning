#include <stdio.h>

#include "ring_buffer.h"

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

static int test_init_valid(void)
{
    RingBuffer buffer;
    unsigned char storage[8];

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 8U));
    CHECK_EQUAL(1, buffer.storage == storage);
    CHECK_EQUAL(8, buffer.capacity);
    CHECK_EQUAL(0, buffer.head);
    CHECK_EQUAL(0, buffer.tail);
    CHECK_EQUAL(0, buffer.overflow);
    return 0;
}

static int test_init_rejects_null_object(void)
{
    unsigned char storage[4];

    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_init(0, storage, 4U));
    return 0;
}

static int test_init_rejects_null_storage(void)
{
    RingBuffer buffer;

    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_init(&buffer, 0, 4U));
    return 0;
}

static int test_init_rejects_capacity_too_small(void)
{
    RingBuffer buffer;
    unsigned char storage[1];

    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_init(&buffer, storage, 1U));
    return 0;
}

static int test_empty_after_init(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char result;
    unsigned char available;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_is_empty(&buffer, &result));
    CHECK_EQUAL(1, result);
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_available(&buffer, &available));
    CHECK_EQUAL(0, available);
    return 0;
}

static int test_single_byte_fifo(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 0x5AU));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(0x5A, value);
    return 0;
}

static int test_multiple_bytes_preserve_fifo_order(void)
{
    RingBuffer buffer;
    unsigned char storage[8];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 8U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 11U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 22U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 33U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(11, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(22, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(33, value);
    return 0;
}

static int test_available_tracks_occupancy(void)
{
    RingBuffer buffer;
    unsigned char storage[8];
    unsigned char available;
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 8U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_available(&buffer, &available));
    CHECK_EQUAL(3, available);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_available(&buffer, &available));
    CHECK_EQUAL(2, available);
    return 0;
}

static int test_fill_uses_capacity_minus_one(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char available;
    unsigned char full;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_available(&buffer, &available));
    CHECK_EQUAL(3, available);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_is_full(&buffer, &full));
    CHECK_EQUAL(1, full);
    return 0;
}

static int test_push_reports_full(void)
{
    RingBuffer buffer;
    unsigned char storage[3];

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 3U));
    return 0;
}

static int test_full_push_does_not_overwrite(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 7U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 8U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 9U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 99U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(7, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(8, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(9, value);
    CHECK_EQUAL(RING_BUFFER_EMPTY, ring_buffer_pop(&buffer, &value));
    return 0;
}

static int test_full_push_sets_overflow_latch(void)
{
    RingBuffer buffer;
    unsigned char storage[2];
    unsigned char overflowed;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_take_overflow(&buffer, &overflowed));
    CHECK_EQUAL(1, overflowed);
    return 0;
}

static int test_take_overflow_clears_latch(void)
{
    RingBuffer buffer;
    unsigned char storage[2];
    unsigned char overflowed;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_take_overflow(&buffer, &overflowed));
    CHECK_EQUAL(1, overflowed);
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_take_overflow(&buffer, &overflowed));
    CHECK_EQUAL(0, overflowed);
    return 0;
}

static int test_indices_wrap(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 4U));
    CHECK_EQUAL(0, buffer.head);
    return 0;
}

static int test_fifo_order_across_wrap(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 10U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 20U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 30U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(10, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 40U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(20, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(30, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(40, value);
    return 0;
}

static int test_recovers_after_full(void)
{
    RingBuffer buffer;
    unsigned char storage[3];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(1, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(2, value);
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(3, value);
    return 0;
}

static int test_reset_restores_empty_state(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char empty;
    unsigned char overflowed;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 3U));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 4U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_reset(&buffer));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_is_empty(&buffer, &empty));
    CHECK_EQUAL(1, empty);
    CHECK_EQUAL(
        RING_BUFFER_OK,
        ring_buffer_take_overflow(&buffer, &overflowed));
    CHECK_EQUAL(0, overflowed);
    return 0;
}

static int test_reset_does_not_clear_storage(void)
{
    RingBuffer buffer;
    unsigned char storage[3];

    storage[0] = 0xA1U;
    storage[1] = 0xB2U;
    storage[2] = 0xC3U;
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 3U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_reset(&buffer));
    CHECK_EQUAL(0xA1, storage[0]);
    CHECK_EQUAL(0xB2, storage[1]);
    CHECK_EQUAL(0xC3, storage[2]);
    return 0;
}

static int test_repeated_fill_and_drain_cycles(void)
{
    RingBuffer buffer;
    unsigned char storage[6];
    unsigned char value;
    unsigned int cycle;
    unsigned int item;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 6U));
    for (cycle = 0U; cycle < 100U; ++cycle)
    {
        for (item = 0U; item < 5U; ++item)
        {
            CHECK_EQUAL(
                RING_BUFFER_OK,
                ring_buffer_push_isr(
                    &buffer,
                    (unsigned char)(cycle + item)));
        }

        for (item = 0U; item < 5U; ++item)
        {
            CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
            CHECK_EQUAL(
                (unsigned char)(cycle + item),
                value);
        }
    }

    return 0;
}

static int test_pop_reports_empty(void)
{
    RingBuffer buffer;
    unsigned char storage[4];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(RING_BUFFER_EMPTY, ring_buffer_pop(&buffer, &value));
    return 0;
}

static int test_smallest_capacity_holds_one_byte(void)
{
    RingBuffer buffer;
    unsigned char storage[2];
    unsigned char value;

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 2U));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_push_isr(&buffer, 0xEEU));
    CHECK_EQUAL(RING_BUFFER_FULL, ring_buffer_push_isr(&buffer, 0xFFU));
    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(0xEE, value);
    return 0;
}

static int test_reset_rejects_null(void)
{
    CHECK_EQUAL(RING_BUFFER_INVALID_ARGUMENT, ring_buffer_reset(0));
    return 0;
}

static int test_pop_rejects_null_output(void)
{
    RingBuffer buffer;
    unsigned char storage[4];

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_pop(&buffer, 0));
    return 0;
}

static int test_queries_reject_null_output(void)
{
    RingBuffer buffer;
    unsigned char storage[4];

    CHECK_EQUAL(RING_BUFFER_OK, ring_buffer_init(&buffer, storage, 4U));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_available(&buffer, 0));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_is_empty(&buffer, 0));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_is_full(&buffer, 0));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_take_overflow(&buffer, 0));
    return 0;
}

static int test_operations_reject_invalid_object(void)
{
    RingBuffer buffer;
    unsigned char value;

    buffer.storage = 0;
    buffer.capacity = 0U;
    buffer.head = 0U;
    buffer.tail = 0U;
    buffer.overflow = 0U;

    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_push_isr(&buffer, 1U));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_pop(&buffer, &value));
    CHECK_EQUAL(
        RING_BUFFER_INVALID_ARGUMENT,
        ring_buffer_available(&buffer, &value));
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
        {"init_valid", test_init_valid},
        {"init_null_object", test_init_rejects_null_object},
        {"init_null_storage", test_init_rejects_null_storage},
        {"init_small_capacity", test_init_rejects_capacity_too_small},
        {"empty_after_init", test_empty_after_init},
        {"single_byte_fifo", test_single_byte_fifo},
        {"multi_byte_fifo", test_multiple_bytes_preserve_fifo_order},
        {"available", test_available_tracks_occupancy},
        {"capacity_minus_one", test_fill_uses_capacity_minus_one},
        {"full_status", test_push_reports_full},
        {"no_overwrite", test_full_push_does_not_overwrite},
        {"overflow_latch", test_full_push_sets_overflow_latch},
        {"take_overflow", test_take_overflow_clears_latch},
        {"index_wrap", test_indices_wrap},
        {"fifo_wrap", test_fifo_order_across_wrap},
        {"full_recovery", test_recovers_after_full},
        {"reset_empty", test_reset_restores_empty_state},
        {"reset_storage", test_reset_does_not_clear_storage},
        {"repeated_cycles", test_repeated_fill_and_drain_cycles},
        {"pop_empty", test_pop_reports_empty},
        {"smallest_capacity", test_smallest_capacity_holds_one_byte},
        {"reset_null", test_reset_rejects_null},
        {"pop_null_output", test_pop_rejects_null_output},
        {"query_null_output", test_queries_reject_null_output},
        {"invalid_object", test_operations_reject_invalid_object}
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
