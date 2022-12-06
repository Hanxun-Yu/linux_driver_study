/**
 *  Created by xunxun on 22-12-6.
 *  芯片相关操作
 *
 *  1.作为platform_driver去注册
 *  2.提供gpio操作的接口给到 led_drv.c
 *
 */


#include "linux/module.h"

#include "linux/fs.h"
#include "linux/errno.h"
#include "linux/miscdevice.h"
#include "linux/kernel.h"
#include <linux/major.h>
#include "linux/mutex.h"
#include "linux/proc_fs.h"
#include "linux/seq_file.h"
#include "linux/stat.h"
#include "linux/init.h"
#include "linux/device.h"
#include "linux/tty.h"
#include "linux/kmod.h"
#include "linux/gfp.h"
#include "linux/platform_device.h"

#include "led_operation.h"
#include "led_drv.h"
#include "chip_gpio.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static s32 board_demo_led_init(u32 which) /* 初始化LED, which-哪个LED */
{
    //printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);

    printk("init gpio: group %d, pin %d\n", GET_GROUP(g_ledpins[which]), GET_PIN(g_ledpins[which]));
    switch (GET_GROUP(g_ledpins[which])) {
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

static s32 board_demo_led_ctl(u32 which, u8 status) /* 控制LED, which-哪个LED, status:1-亮,0-灭 */{
    //printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
    printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GET_GROUP(g_ledpins[which]),
           GET_PIN(g_ledpins[which]));

    switch (GET_GROUP(g_ledpins[which])) {
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

static struct led_api board_demo_led_opr = {
        .led_init = board_demo_led_init,
        .led_ctrl  = board_demo_led_ctl,
};

/**
 * 驱动配对成功后会调用这个函数
 * @param pdev
 * @return
 */
static int chip_demo_gpio_probe(struct platform_device *pdev) {
    struct resource *res;
    int i = 0;

    while (1) {
        //从device那里获取资源
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i++);
        if (!res)
            break;

        //取出pin值
        g_ledpins[g_ledcnt] = res->start;
        //创建设备节点
        led_class_create_device(g_ledcnt);
        g_ledcnt++;
    }
    return 0;

}

/**
 * 驱动卸载时将调用
 * @param pdev
 * @return
 */
static int chip_demo_gpio_remove(struct platform_device *pdev) {
    struct resource *res;
    int i = 0;

    while (1) {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if (!res)
            break;

        led_class_destroy_device(i);
        i++;
        g_ledcnt--;
    }
    return 0;
}


static struct platform_driver chip_demo_gpio_driver = {
        .probe      = chip_demo_gpio_probe,
        .remove     = chip_demo_gpio_remove,
        .driver     = {
                .name   = "xunxun_led",
        },
};

static int __init chip_demo_gpio_drv_init(void) {
    int err;

    err = platform_driver_register(&chip_demo_gpio_driver);
    register_led_operations(&board_demo_led_opr);

    return 0;
}

static void __exit chip_demo_gpio_drv_exit(void) {
    platform_driver_unregister(&chip_demo_gpio_driver);
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");

