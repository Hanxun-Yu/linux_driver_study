//
// Created by xunxun on 22-12-5.
//
#include "led_resource.h"

/**
 * 以后换了板子，资源发生变化
 * 只需要修改这个文件，或者添加一个新的board_X_led.c加入编译即可
 */

static led_resource board_A_led = {
//        .pin = (3 << 16) | (1 << 0) //表示第3组，第1个引脚
        .group_pin = GROUP_PIN(3, 1)
};

led_resource *get_led_resource(void) {
    return &board_A_led;
}