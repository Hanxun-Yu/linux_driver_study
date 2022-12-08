#include "linux/module.h"
#include "asm/io.h"
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
#include "../inc/button_drv.h"


/**
 * 结构体中的变量定义类型和顺序，等于已经把地址偏移好了
 */
struct stm32mp157_gpio {
    volatile unsigned int MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    volatile unsigned int OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    volatile unsigned int OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    volatile unsigned int PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    volatile unsigned int IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    volatile unsigned int ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    volatile unsigned int BSRR;     /*!< GPIO port bit set/reset,               Address offset: 0x18      */
    volatile unsigned int LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    volatile unsigned int AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
};

/**
 * 查看原理图,板载2个按键，读取这2个按键的引脚分别为 PG3 PG2
 * PG3 表示GPIO第G组 第3个引脚
 */


//查看stm32mp157芯片手册 2.5.2 Peripheral mapping 查看gpio基址
const u32 RCC_BASE_phy = 0x50000000; // 162页 AHB4,最后一行， 0x50000000 - 0x50000FFF 4 KB RCC
const u32 GPIO_G_BASE_phy  = 0x50008000;// 162页 AHB4,最后12行， 0x50008000 - 0x500083FF 1 KB GPIOG

/* register 物理地址*/
const u32 RCC_PLL4CR_phy = RCC_BASE_phy + 0x894;//686页 10.7.42 使能时钟PLL4, 使能后gpio模块才可用
const u32 RCC_MP_AHB4ENSETR_phy = RCC_BASE_phy + 0xA28; //869页 10.7.155 AHB4寄存器

/* 虚拟地址 */
volatile u32 *RCC_PLL4CR_virtual;
volatile u32 *RCC_MP_AHB4ENSETR_virtual;

static struct stm32mp157_gpio *gpiog; /* KEY1: PG3, KEY2: PG2 */


static void stm32mp157_button_init(int which) {
    printk(KERN_DEBUG "%s %s line %d,button %d\n", __FILE__, __FUNCTION__, __LINE__, which);

    if (!RCC_PLL4CR_virtual) {
        /* ioremap 转换为虚拟地址 */
        RCC_PLL4CR_virtual = ioremap(RCC_PLL4CR_phy, 4);
        RCC_MP_AHB4ENSETR_virtual = ioremap(RCC_MP_AHB4ENSETR_phy, 4);
        /*使能PLL4 */
        //bit0置1,
        *RCC_PLL4CR_virtual |= (1 << 0);
        //等待bit1变1
        while ((*RCC_PLL4CR_virtual & (1 << 1)) == 0) {
            printk("PLL4 not locker\n");
        }

        gpiog = ioremap(GPIO_G_BASE_phy, sizeof(struct stm32mp157_gpio));

    }

    if (which == 0) {

        /* enable GPIOG */
        *RCC_MP_AHB4ENSETR_virtual |= (1 << 6);

        /*
         * 1077页 13.4.1
         * 设置PG3为GPIO模式, 输入模式
         * 从表上看PG3在寄存器 bit[6:7]
         * 输入模式，把这2位置0
         *
         */
        gpiog->MODER &= ~(0b11 << 6); // 取反后变成00111111

    } else if (which == 1) {

        //同理
        *RCC_MP_AHB4ENSETR_virtual |= (1 << 6);
        gpiog->MODER &= ~(0b11 << 4); // 取反后变成11001111
    }

}

static int stm32mp157_button_read(int which) {
    int status = -1;
    if (which == 0) {
        status = (gpiog->IDR & (1 << 3)) == (1 << 3);

    } else if (which == 1) {
        status = (gpiog->IDR & (1 << 2)) == (1 << 2);
    }
    return status;
}


static struct button_operations my_buttons_ops = {
        .count = 2,
        .init = stm32mp157_button_init,
        .read = stm32mp157_button_read,
};

static int stm32mp157_button_drv_init(void) {
    register_button_operations(&my_buttons_ops);
    return 0;
}

static void stm32mp157_button_drv_exit(void) {
    unregister_button_operations();
}

module_init(stm32mp157_button_drv_init);
module_exit(stm32mp157_button_drv_exit);
MODULE_LICENSE("GPL");



