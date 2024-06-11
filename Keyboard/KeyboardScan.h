#ifndef KEYBOARDSCAN_H
#define KEYBOARDSCAN_H

#include "main.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct Keyboard_GPIO_t
{
    GPIO_TypeDef * port;
    uint32_t pin;
};

void KeyboardInit();

/**
 * @brief 在systick中断中进行回调
 * 
 */
void KeyboardScan_Callback(void);

/**
 * @brief 在主函数的while循环中调用，当需要更新数据的时候自动发送
 * 
 */
void KeyboardSend(void);

#if defined(__cplusplus)
}
#endif

#endif // KEYBOARDSCAN_H