//
// Created by xunxun on 22-12-6.
//

#ifndef DEMO_LED_OPERATION_H
#define DEMO_LED_OPERATION_H
#include "asm/types.h"

struct led_api {
    s32 (*led_init)(u32 group_pin);

    s32 (*led_ctrl)(u32 group_pin, u8 status);
};

struct led_api *get_led_api(void);

#endif //DEMO_LED_OPERATION_H
