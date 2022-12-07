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
#include "linux/of.h"


#include "led_operation.h"
#include "led_drv.h"
#include "chip_gpio.h"

static s32 g_ledpins[100];
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
    struct device_node *np = pdev->dev.of_node;

    if (!np) {
        //这个pdev 不是来自设备树
        return -1;
    }


    //读取设备节点内的属性
    u32 led_pin;
    int err = of_property_read_u32(np, "pin", &led_pin);

    printk("of_property_read_u32 err:%d\n",err);
    if (!err) {
        printk("ARRAY_SIZE(g_ledpins):%u\n",ARRAY_SIZE(g_ledpins));
        //取出pin值
        u32 i;
        for (i = 0; i < ARRAY_SIZE(g_ledpins); i++) {
            printk("g_ledpins[%d]=%d\n",i,g_ledpins[i]);
            //找1个等于-1的位置，说明没有人用
            if (g_ledpins[i] == -1) {
                g_ledpins[i] = (s32) led_pin;
                //创建设备节点
                led_class_create_device(g_ledcnt);
                g_ledcnt++;
                printk("led_class_create_device\n");
                break;
            }
        }

    }
    return 0;

}

/**
 * 驱动卸载时将调用
 * @param pdev
 * @return
 */
static int chip_demo_gpio_remove(struct platform_device *pdev) {
    struct device_node *np = pdev->dev.of_node;
    u32 led_pin;
    int err = of_property_read_u32(np, "pin", &led_pin);

    if (!err) {
        u32 i = 0;
        for (i = 0; i < g_ledcnt; i++) {
            if (g_ledpins[i] == led_pin) {
                led_class_destroy_device(i);
                g_ledpins[i] = -1;
                g_ledcnt--;
                break;
            }
        }
    }

    return 0;
}

static const struct of_device_id xunxun_leds[] = {
        {.compatible = "xunxun,led_drv"},
        {},
};

static struct platform_driver chip_demo_gpio_driver = {
        .probe      = chip_demo_gpio_probe, //每匹配1个platform_device就会调用一次probe函数
        .remove     = chip_demo_gpio_remove,
        .driver     = {
                .name   = "xunxun_led",
                .of_match_table = xunxun_leds, //使用设备树，必须添加这一项，与设备树中的compatible属性对应
        },
};

static int __init chip_demo_gpio_drv_init(void) {
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    int err;
    //把所有pin初始化为-1
    int i = 0;
    for (i = 0; i < ARRAY_SIZE(g_ledpins); i++) {
        g_ledpins[i] = -1;
    }

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

