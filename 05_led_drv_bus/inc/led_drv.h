//
// Created by xunxun on 22-12-6.
//


#ifndef _LEDDRV_H
#define _LEDDRV_H

#include "chip_gpio.h"
#include "led_operation.h"

//用于被platform_driver调用
void led_class_create_device(int minor);
void led_class_destroy_device(int minor);

//芯片相关类调用这个函数，把芯片相关操作的具体实现设置进来
void register_led_operations(struct led_api *api);

#endif /* _LEDDRV_H */