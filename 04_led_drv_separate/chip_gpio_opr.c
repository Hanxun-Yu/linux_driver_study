//
// Created by xunxun on 22-12-5.
//

#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#include "chip_gpio_opr.h"


static s32 gpio_init(u32 group_pin) /* 初始化LED, which-哪个LED */
{
    //printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);

    printk("init gpio: group %d, pin %d\n", GET_GROUP(group_pin), GET_PIN(group_pin));
    switch (GET_GROUP(group_pin)) {
        case 0: {
            printk("init pin of group 0 ...\n");
            break;
        }
        case 1: {
            printk("init pin of group 1 ...\n");
            break;
        }
        case 2: {
            printk("init pin of group 2 ...\n");
            break;
        }
        case 3: {
            printk("init pin of group 3 ...\n");
            break;
        }
    }

    return 0;
}

static s32 gpio_ctrl(u32 group_pin, u8 status) /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
{
    //printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
    printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GET_GROUP(group_pin), GET_PIN(group_pin));

    switch (GET_GROUP(group_pin)) {
        case 0: {
            printk("set pin of group 0 ...\n");
            break;
        }
        case 1: {
            printk("set pin of group 1 ...\n");
            break;
        }
        case 2: {
            printk("set pin of group 2 ...\n");
            break;
        }
        case 3: {
            printk("set pin of group 3 ...\n");
            break;
        }
    }

    return 0;
}

static struct gpio_api opr = {
        .gpio_init = gpio_init,
        .gpio_ctrl  = gpio_ctrl,
};

struct gpio_api *get_gpio_api(void) {
    return &opr;
}