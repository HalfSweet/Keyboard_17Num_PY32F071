#ifndef WS2812_H
#define WS2812_H

#include "py32f071_ll_bus.h"
#include "py32f071_ll_spi.h"
#include "py32f071_ll_gpio.h"

constexpr uint16_t LIGHT_COUNT = 17;

class WS2812
{
public:
    WS2812(SPI_TypeDef *spi = SPI1, const uint16_t count = 1)
        : _spi(spi), _count(count){};
    ~WS2812(){};

    /**
     * @brief 初始化WS2812，在调用之前需要手动初始化SPI
     * 
     */
    void Init();

    /**
     * @brief 更新数据，在进行任何更改之后需要调用才能真正改变灯的颜色
     * 
     */
    void Update();

    /**
     * @brief 设置灯的颜色
     * 
     * @param index 灯的编号
     * @param R 红色
     * @param G 绿色
     * @param B 蓝色
     */
    void SetColor(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

private:
    SPI_TypeDef *_spi;
    uint16_t _count;

    struct WS2812_RGB_t
    {
        uint8_t G;
        uint8_t R;
        uint8_t B;
    };

    WS2812_RGB_t _color[LIGHT_COUNT];
};

#endif