//
// Created by xunxun on 22-12-6.
// 定义板载资源
//

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

#include "chip_gpio.h"


static void led_pdev_release(struct device *dev) {
}

static struct resource resources[] = {
        {
                .start = GROUP_PIN(3, 1), //资源数据，自定义
                .flags = IORESOURCE_IRQ, //资源类型，可以自定义，driver那边取的时候对应即可
                .name = "xunxun_led_pin",//名称
        },
        {
                .start = GROUP_PIN(5, 8),
                .flags = IORESOURCE_IRQ,
                .name = "xunxun_led_pin",
        },
};


static struct platform_device board_A_led_pdev = {
        .name = "xunxun_led",
        .num_resources = ARRAY_SIZE(resources),
        .resource = resources,
        .dev = {
                .release = led_pdev_release,
        },
};

static int __init led_pdev_init(void) {
    int err;

    err = platform_device_register(&board_A_led_pdev);

    return 0;
}

static void __exit led_pdev_exit(void) {
    platform_device_unregister(&board_A_led_pdev);
}

module_init(led_pdev_init);
module_exit(led_pdev_exit);

MODULE_LICENSE("GPL");

