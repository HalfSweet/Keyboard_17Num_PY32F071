#include "Ringbuffer.h"

RingBuffer::RingBuffer()
    : head(0), tail(0)
{
}

RingBuffer::~RingBuffer()
{
}

bool RingBuffer::write(uint8_t *data, int len)
{
    if (len > BUFFER_SIZE)
    {
        return false;
    }

    // 如果缓冲区剩余空间不足，写入失败
    if (len > BUFFER_SIZE - length())
    {
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        buffer[head] = data[i];
        head = (head + 1) % BUFFER_SIZE;
    }

    return true;
}

bool RingBuffer::read(uint8_t *data, int len)
{
    if (len > BUFFER_SIZE)
    {
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        data[i] = buffer[tail];
        // 读取后将数据清零
        buffer[tail] = 0;
        tail = (tail + 1) % BUFFER_SIZE;
    }

    return true;
}

bool RingBuffer::put(uint8_t data)
{
    if (length() == BUFFER_SIZE)
    {
        return false;
    }

    buffer[head] = data;
    head = (head + 1) % BUFFER_SIZE;

    return true;
}

int RingBuffer::length()
{
    return (head - tail + BUFFER_SIZE) % BUFFER_SIZE;
}

int RingBuffer::capacity()
{
    return BUFFER_SIZE;
}
