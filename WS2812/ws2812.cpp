#include "ws2812.h"
#include <cstdint>

void WS2812::Init()
{
    // 将缓存初始化为0，并发送到灯上
    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        _color[i].R = 0x00;
        _color[i].G = 0x00;
        _color[i].B = 0x00;
    }
    Update();
}

void WS2812::Update()
{
    // 发送数据
    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        uint16_t bits = 24;
        uint32_t value = 0x00000000;
        value = (_color[i].G << 16) | (_color[i].R << 8) | _color[i].B;
        while (bits > 0)
        {
            uint8_t temp = 0;
            if ((value & 0x800000) != 0)
            {
                temp = 0xF8;
            }
            else
            {
                temp = 0xE0;
            }
            LL_SPI_TransmitData8(_spi, temp);
            while (LL_SPI_IsActiveFlag_TXE(_spi) == 0)
            {
                ;
            }
            value <<= 1;
            bits--;
        }
    }
}

void WS2812::SetColor(uint16_t index, uint8_t R, uint8_t G, uint8_t B)
{
    _color[index].B = B;
    _color[index].G = G;
    _color[index].R = R;
}