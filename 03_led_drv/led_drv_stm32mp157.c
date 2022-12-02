#include <linux/module.h>
#include <asm/io.h>
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
#include "led_opr.h"

/* register 物理地址*/
const u32 RCC_PLL4CR_phy = 0x50000000 + 0x894;//使能时钟PLL4, 使能后gpio模块才可用
const u32 RCC_MP_AHB4ENSETR_phy = 0x50000000 + 0xA28; //A7芯片 使能 gpioA组 PA10 : gpioA 第10个引脚
// u32 RCC_MC_AHB4ENSETR_phy = 0x50000000 + 0xAA8; //M4芯片 使能 gpioA组 PA10 : gpioA 第10个引脚
const u32 GPIOA_MODER_phy = 0x50002000 + 0x00;//GPIO 输入/输出模式
const u32 GPIOA_BSRR_phy = 0x50002000 + 0x18;//控制GPIO 输出 0/1

/* 虚拟地址 */
volatile u32 *RCC_PLL4CR_virtual;
volatile u32 *RCC_MP_AHB4ENSETR_virtual;
volatile u32 *GPIOA_MODER_virtual;
volatile u32 *GPIOA_BSRR_virtual; //使用volatile修饰，表示该变量不要在编译时被优化（不加的话，比如连续两次赋值，编译器会优化掉第一次赋值）

static int stm32mp157_led_init(int which) {
    printk(KERN_DEBUG "%s %s line %d,led %d\n", __FILE__, __FUNCTION__, __LINE__, which);

    if (!RCC_PLL4CR_virtual) {
        /* ioremap 转换为虚拟地址 */
        RCC_PLL4CR_virtual = ioremap(RCC_PLL4CR_phy, 4);
        /*使能PLL4 */
        //bit0置1,
        *RCC_PLL4CR_virtual |= (1 << 0);
        //等待bit1变1
        while ((*RCC_PLL4CR_virtual & (1 << 1)) == 0) {
            printk("PLL4 not locker\n");
        }
    }


    //若是led0
    //这个序号0是在led_drv.c中注册的次设备号
    //现在在当前这个文件中决定了序号0控制的是PA10引脚连接的led
    if (which == 0) {
        if (!RCC_MP_AHB4ENSETR_virtual) {
            /* ioremap 转换为虚拟地址
                ioremap(base_phy), size);
            */
            RCC_MP_AHB4ENSETR_virtual = ioremap(RCC_MP_AHB4ENSETR_phy, 4);
            GPIOA_MODER_virtual = ioremap(GPIOA_MODER_phy, 4);
            GPIOA_BSRR_virtual = ioremap(GPIOA_BSRR_phy, 4);
        }

        /*使能GPIOA */
        //bit0置1
        *RCC_MP_AHB4ENSETR_virtual |= (1 << 0);

        /*配置PA10 为输出*/
        //bit 20 21 清0
        *GPIOA_MODER_virtual &= ~(0b11 << 20);
        //bit 20置1 21置0
        *GPIOA_MODER_virtual |= (0b01 << 20);
    }
    return 0;
}

static int stm32mp157_led_release(int which) {
    printk(KERN_DEBUG "%s %s line %d,led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
    /* iounmap
         iounmap(address_virtual);
    */
    iounmap(RCC_PLL4CR_virtual);

    if (which == 0) {
        if (RCC_MP_AHB4ENSETR_virtual) {
            iounmap(RCC_MP_AHB4ENSETR_virtual);
            iounmap(GPIOA_MODER_virtual);
            iounmap(GPIOA_BSRR_virtual);
        }
    }
    return 0;
}

static int stm32mp157_led_ctl(int which, char status) {
    printk(KERN_DEBUG "%s %s line %d,led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");

    /*gpio PA10 置0， 则led会亮*/
    //这里使用set/reset register
    //bit0-15 用来set(是置1的)   bit16-31 用来reset(是置0的)
    if (status) {
        //on
        //根据原理图，如果要点亮，要置0，所以使用BR[15:0] 即bit16-31，R表示reset，这个寄存器置1，则会把gpio置0
        // 把其中表示PA10的那一位置1，则实际物理寄存器会置0
        *GPIOA_BSRR_virtual |= (1 << (16 + 10));


    } else {
        //off
        //根据原理图，如果要熄灭，要置1，所以使用BR[15:0] bit0-15，S表示set，这个寄存器置1，则会把gpio置1
        // 把其中表示PA10的那一位置1，则实际物理寄存器会置1
        *GPIOA_BSRR_virtual |= (1 << 10);

    }
    return 0;
}

static struct led_operations stm32mp157_led_opr = {
        .num = 1, //表示stm32mp157 上有多少led
        .init = stm32mp157_led_init,
        .ctl = stm32mp157_led_ctl,
        .release = stm32mp157_led_release
};

struct led_operations *get_board_led_opr(void) {
    return &stm32mp157_led_opr;
};


