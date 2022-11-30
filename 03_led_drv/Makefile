#KERN_DIR = /home/xunxun/100ask_stm32mp157_pro-sdk/Linux-5.4
KERN_DIR = ../Linux-5.4_link

CFLAGS += -I ../Linux-5.4_link/include # 指定头文件搜索路径

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o led_drv_test led_drv_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f led_drv_test

# 参考内核源码drivers/char/ipmi/Makefile
# 要想把a.c, b.c编译成ab.ko, 可以这样指定:
# ab-y := a.o b.o
# obj-m += ab.o

# leddrv.c led_drv_stm32mp157.c 编译成 xunxun_led.ko
xunxun_led-y := led_drv.o led_drv_stm32mp157.o
obj-m	+= xunxun_led.o