#include "linux/module.h"

#include "linux/device.h"
#include "linux/errno.h"
#include "linux/fs.h"
#include "linux/gfp.h"
#include "linux/init.h"
#include "linux/kernel.h"
#include "linux/kmod.h"
#include <linux/major.h>
#include "linux/miscdevice.h"
#include "linux/mutex.h"
#include "linux/proc_fs.h"
#include "linux/seq_file.h"
#include "linux/stat.h"
#include "linux/tty.h"
#include "../inc/button_drv.h"


/* 这个驱动主要实现了存放上层app传入的字符串，提供读写*/

// 1.确定主设备号
static int major = 0;
static struct class *button_class;
struct button_operations *p_button_opr;

#define MIN(a, b) (a < b ? a : b)

// 3.实现对应的open/read/write等函数，填入file_operations结构体
static ssize_t button_drv_read(struct file *f,
                               char __user *buf,
                               size_t size,
                               loff_t *offset) {
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    int status;
    struct inode *node;
    u32 minor;

    //根据open时指定的次设备号与status，来控制led
    // 次设备号就是  /dev/xunxun_led0 中的 0
    node = file_inode(f);
    minor = iminor(node);
    status = p_button_opr->read((s32) minor);

    u64 err = copy_to_user(buf, &status, 4);

    return 0;
}

static int button_drv_open(struct inode *node, struct file *f) {
    u32 minor;
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    //根据次设备号初始化led
    minor = iminor(node);
    p_button_opr->init((s32) minor);
    return 0;
}

// 2.定义自己的file_operations结构体
static struct file_operations led_drv = {.owner = THIS_MODULE,
        .open = button_drv_open,
        .read = button_drv_read,
};

// 4.把file_operations结构体告诉内核:注册驱动程序

// 5.谁来注册驱动程序？得有一个入口函数:安装驱动程序时，就会去调用这个入口函数
static int __init button_drv_init(void) {
    s64 err;
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    major = register_chrdev(0, "xunxun_button", &led_drv);

    button_class = class_create(THIS_MODULE, "xunxun_button_class");
    err = PTR_ERR(button_class);
    if (IS_ERR(button_class)) {
        printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

// 6.有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
static void __exit button_drv_exit(void) {
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    class_destroy(button_class);
    unregister_chrdev(major, "xunxun_button");
}

void register_button_operations(struct button_operations *opr) {
    int i;

    p_button_opr = opr;
    for (i = 0; i < opr->count; i++) {
        device_create(button_class, NULL, MKDEV(major, i),
                      NULL, "xunxun_button%d", i);
    }
}

void unregister_button_operations(void) {
    int i;

    for (i = 0; i < p_button_opr->count; i++) {
        device_destroy(button_class, MKDEV(major, i));
    }
}

EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);
// 7.其他完善：提供设备信息，自动创建设备节点

module_init(button_drv_init);  //注册
module_exit(button_drv_exit);  //注册
MODULE_LICENSE("GPL");