//
// Created by xunxun on 22-12-5.
//

#pragma once

#include "asm/types.h"
#include "chip_gpio_opr.h"

typedef struct gpio_resource led_resource;

led_resource *get_led_resource(void);
