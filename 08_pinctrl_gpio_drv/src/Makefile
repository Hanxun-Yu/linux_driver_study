mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
LOCAL_PATH := $(mkfile_dir)

#KERN_DIR = /home/xunxun/100ask_stm32mp157_pro-sdk/Linux-5.4
KERN_DIR = ../../Linux-5.4_link

ccflags-y += -I ../../Linux-5.4_link/include # 指定头文件搜索路径
ccflags-y += -I $(LOCAL_PATH)/../inc # 指定头文件搜索路径
ccflags-y += -Wno-declaration-after-statement #去除警告：每个函数变量必须定义在函数开头
ccflags-y += -Werror #将警告定义为错误，编译将终止

all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o test test.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f test

# 参考内核源码drivers/char/ipmi/Makefile
# 要想把a.c, b.c编译成ab.ko, 可以这样指定:
# ab-y := a.o b.o
# obj-m += ab.o


obj-m := led_drv.o
