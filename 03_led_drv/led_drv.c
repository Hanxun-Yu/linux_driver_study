#include <linux/module.h>

#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/major.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/tty.h>
#include "led_opr.h"


/* 这个驱动主要实现了存放上层app传入的字符串，提供读写*/

// 1.确定主设备号
static int major = 0;
static struct class *led_class;
struct led_operations *p_led_opr;

#define MIN(a, b) (a < b ? a : b)

// 3.实现对应的open/read/write等函数，填入file_operations结构体
static ssize_t led_drv_read(struct file *f,
                            char __user *buf,
                            size_t size,
                            loff_t *offset) {
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/* app 会这样调用 
	write(fd, &val, 1); */
static ssize_t led_drv_write(struct file *f,
                             const char __user *val,
                             size_t size,
                             loff_t *offset) {
    //linux驱动中，一定要把所有变量定义放在函数开头，否则会出如下警告
    // warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
    int err;
    char status;//接收用户输入
    struct inode *node;
    int minor;

    err = copy_from_user(&status, val, 1);
    //根据open时指定的次设备号与status，来控制led
    // 次设备号就是  /dev/xunxun_led0 中的 0
    node = file_inode(f);
    minor = iminor(node);
    p_led_opr->ctl(minor, status);


    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 1;
}

static int led_drv_open(struct inode *node, struct file *f) {
    int minor;
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    //根据次设备号初始化led
    minor = iminor(node);
    p_led_opr->init(minor);
    return 0;
}

static int led_drv_release(struct inode *node, struct file *f) {
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

// 2.定义自己的file_operations结构体
static struct file_operations led_drv = {.owner = THIS_MODULE,
        .open = led_drv_open,
        .read = led_drv_read,
        .write = led_drv_write,
        .release = led_drv_release};

// 4.把file_operations结构体告诉内核:注册驱动程序

// 5.谁来注册驱动程序？得有一个入口函数:安装驱动程序时，就会去调用这个入口函数
static int __init led_init(void) {
    int err;
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    major = register_chrdev(0, "xunxun_led", &led_drv);

    led_class = class_create(THIS_MODULE, "xunxun_led_class");
    err = PTR_ERR(led_class);
    if (IS_ERR(led_class)) {
        printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
        unregister_chrdev(major, "led");
        return -1;
    }
    p_led_opr = get_board_led_opr();

    int i = 0;
    for (i = 0; i < p_led_opr->num; i++) {
        device_create(led_class, NULL, MKDEV(major, i), NULL, "xunxun_led%d",
                      i);  //创建设备节点 /dev/xunxun_led0
    }


    return 0;
}

// 6.有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
static void __exit led_exit(void) {
    printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    p_led_opr->release(0);
    int i;
    for (i = 0; i < p_led_opr->num; i++) {
        device_destroy(led_class, MKDEV(major, i));
    }
    class_destroy(led_class);


    unregister_chrdev(major, "led");
}

// 7.其他完善：提供设备信息，自动创建设备节点

module_init(led_init);  //注册
module_exit(led_exit);  //注册
MODULE_LICENSE("GPL");