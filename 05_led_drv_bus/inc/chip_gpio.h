//
// Created by xunxun on 22-12-5.
//

#pragma once

#include "asm/types.h"

//定义一些宏方便解析或创建
#define GET_GROUP(x) (x >> 16)
#define GET_PIN(x) (x & 0xffff)
#define GROUP_PIN(g, p) ((g << 16) | (p & 0xffff))

struct gpio_resource {
    /**
     * 高16位表示gpio组号
     * 低16位表示组内gpio序号
     * bit[31:16] = group
     * bit[15:0] = which pin
     *
     * 合起来用一个32位整形表示
     */
    u32 group_pin;
};
