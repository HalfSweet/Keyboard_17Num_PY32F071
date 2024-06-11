#include "KeyboardScan.h"

#include <cstdint>
#include <cstring>
#include <stdint.h>

#include "hid_keyboard.h"
#include "usbd_core.h"
#include "usbd_hid.h"

#include "py32f071_ll_bus.h"
#include "py32f071_ll_gpio.h"

// 按键缓存。每一个按键都占有一个唯一的index，这个index就是这个按键在这个数组中的下标，缓存最多存储6个按键。
// 当按键弹起时，清理这个缓存。

static uint8_t buffer[6];
static bool update_flag = false;

static uint8_t layout[5][4] = {
    {HID_KBD_USAGE_KPDNUMLOCK, HID_KBD_USAGE_KPDDIV, HID_KBD_USAGE_KPDMUL, HID_KBD_USAGE_KPDHMINUS},
    // 7 8 9 +
    {HID_KBD_USAGE_KPDHOME, HID_KBD_USAGE_KPDUP, HID_KBD_USAGE_KPDPAGEUP, HID_KBD_USAGE_KPDPLUS},
    // 4 5 6
    {HID_KBD_USAGE_KPDLEFT, 0x5d, HID_KBD_USAGE_KPDRIGHT},
    // 1 2 3 enter
    {HID_KBD_USAGE_KPDEND, HID_KBD_USAGE_KPDDOWN, HID_KBD_USAGE_KPDPAGEDN, HID_KBD_USAGE_KPDEMTER},
    // 0 ./del
    {HID_KBD_USAGE_KPD0, 0, HID_KBD_USAGE_KPDDELETE}};

static const Keyboard_GPIO_t row[5] = {
    {GPIOA, GPIO_PIN_0},
    {GPIOA, GPIO_PIN_1},
    {GPIOA, GPIO_PIN_2},
    {GPIOA, GPIO_PIN_3},
    {GPIOA, GPIO_PIN_4}};

static const Keyboard_GPIO_t col[4] = {
    {GPIOA, GPIO_PIN_5},
    {GPIOA, GPIO_PIN_6},
    {GPIOA, GPIO_PIN_7},
    {GPIOA, GPIO_PIN_8}};

// 每个按键被扫描到的次数
static uint8_t key_count[5][4] = {0};

// 抬起指示
static bool key_up[5][4] = {0};

void KeyboardInit()
{
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    for (auto i : row)
    {
        // 都拉低
        LL_GPIO_ResetOutputPin(i.port, i.pin);
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = i.pin;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
        LL_GPIO_Init(i.port, &GPIO_InitStruct);
    }

    for (auto i : col)
    {
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = i.pin;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
        LL_GPIO_Init(i.port, &GPIO_InitStruct);
    }
}

void KeyboardScan_Callback(void)
{
    // 逐行扫描，每次扫描一行，如果每行连续5次都存在按键按下，则认为按下
    static uint8_t nowRow = 0;
    // 拉高这一行
    LL_GPIO_SetOutputPin(row[nowRow].port, row[nowRow].pin);

    // 检查这一行的按键
    {
        uint8_t index = 0;
        for (auto i : col)
        {
            if (LL_GPIO_IsInputPinSet(i.port, i.pin))
            {
                key_count[nowRow][index]++;
            }
            else
            {
                key_count[nowRow][index] = 0;
                // 如果这个按键之前被按下过，则将这个按键的键码从buffer中清除
                for (int j = 0; j < 6; j++)
                {
                    if (buffer[j] == layout[nowRow][index])
                    {
                        buffer[j] = 0;
                        update_flag = true;
                        break;
                    }
                }
            }
            index++;
        }
    }

    // 如果有按键超过5次被按下，则将这个按键的键码加入到环形缓冲区中
    for (int i = 0; i < 4; i++)
    {
        if (key_count[nowRow][i] > 5)
        {
            auto layout_index = layout[nowRow][i];
            if (layout_index == 0)
            {
                continue;
            }

            // 检查这个按键是否已经被按下
            bool isExist = false;
            for (int j = 0; j < 6; j++)
            {
                if (buffer[j] == layout_index)
                {
                    isExist = true;
                    break;
                }
            }

            // 如果这个按键没有被按下，则将这个按键的键码加入到buffer中
            if (!isExist)
            {
                for (int j = 0; j < 6; j++)
                {
                    if (buffer[j] == 0)
                    {
                        buffer[j] = layout_index;
                        update_flag = true;
                        break;
                    }
                }
            }
        }
    }

    // 拉低这一行
    LL_GPIO_ResetOutputPin(row[nowRow].port, row[nowRow].pin);
    nowRow++;
    nowRow %= 5;
}

void KeyboardSend(void)
{
    // while (hid_state == HID_STATE_BUSY)
    // {
    // }

    if (update_flag)
    {
        update_flag = false;
        // 将buffer中的内容发送到USB
        hid_keyboard_send(0, buffer);
    }
}