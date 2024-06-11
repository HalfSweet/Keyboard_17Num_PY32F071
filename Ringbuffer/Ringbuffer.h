/**
 * @file Ringbuffer.h
 * @author HalfSweet
 * @brief 一个简单的环形缓冲区实现，最大长度为128字节
 * @version 0.1
 * @date 2024-06-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

class RingBuffer
{
public:
    RingBuffer();
    ~RingBuffer();

    /**
     * @brief 向环形缓冲区中写入数据
     * 
     * @param data 要写入的数据
     * @param len 数据长度
     * @return true 写入成功
     * @return false 写入失败
     */
    bool write(uint8_t *data, int len);

    /**
     * @brief 从环形缓冲区中读取数据
     * 
     * @param data 读取数据的存储地址
     * @param len 读取数据的长度
     * @return true 读取成功
     * @return false 读取失败
     */
    bool read(uint8_t *data, int len);

    /**
     * @brief 从环形缓冲区中写入一个字节
     * 
     * @param data  要写入的数据
     * @return true 写入成功
     * @return false 写入失败
     */
    bool put(uint8_t data);

    /**
     * @brief 获取环形缓冲区中的数据长度
     * 
     * @return int 数据长度
     */
    int length();

    /**
     * @brief 获取环形缓冲区的容量
     * 
     * @return int 容量
     */
    int capacity();

private:
    static constexpr uint16_t BUFFER_SIZE = 128;
    uint8_t buffer[BUFFER_SIZE];
    int head;
    int tail;
};


#endif // RINGBUFFER_HUFFER