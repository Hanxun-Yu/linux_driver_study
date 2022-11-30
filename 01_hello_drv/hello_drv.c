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

/* 这个驱动主要实现了存放上层app传入的字符串，提供读写*/

//1.确定主设备号
static int major = 0;
static char kernel_buf[1024]; //用来存放用户输入给驱动的数据
static struct class *hello_class;

#define MIN(a, b) (a < b ? a : b)

//3.实现对应的open/read/write等函数，填入file_operations结构体
static ssize_t hello_drv_read (struct file *f, char __user *buf, size_t size, loff_t *offset) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	u32 len = copy_to_user(buf, kernel_buf, MIN(1024,size));
	return len;

}
static ssize_t hello_drv_write (struct file *f, const char __user *buf, size_t size, loff_t *offset) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__ ,__FUNCTION__, __LINE__);
	u32 len = copy_from_user(kernel_buf, buf, MIN(1024,size));
	return len;

}
static int hello_drv_open (struct inode *node, struct file *f) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;

}
static int hello_drv_release (struct inode *node, struct file *f) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}





//2.定义自己的file_operations结构体
static struct file_operations hello_drv = {
	.owner = THIS_MODULE,
	.open = hello_drv_open,
	.read = hello_drv_read,
	.write = hello_drv_write,
	.release = hello_drv_release
};


//4.把file_operations结构体告诉内核:注册驱动程序



//5.谁来注册驱动程序？得有一个入口函数:安装驱动程序时，就会去调用这个入口函数
static int __init hello_init(void) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	major = register_chrdev(0, "hello", &hello_drv);
	
	hello_class = class_create(THIS_MODULE, "hello_class");
	int err = PTR_ERR(hello_class);
	if (IS_ERR(hello_class)) {
		printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "hello");
		return -1;
	}

	device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello");

	
	return 0;
}

//6.有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
static void __exit hello_exit(void) {
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	device_destroy(hello_class, MKDEV(major, 0));
    class_destroy(hello_class);
	unregister_chrdev(major, "hello");
	return 0;
}


//7.其他完善：提供设备信息，自动创建设备节点


module_init(hello_init); //注册
module_exit(hello_exit); //注册
MODULE_LICENSE("GPL");