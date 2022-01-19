KERN_DIR = /home/xunxun/100ask_stm32mp157_pro-sdk/Linux-5.4

CFLAGS += -I ../Linux-5.4/include # 指定头文件搜索路径

all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o hello_drv_test hello_drv_test.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f hello_drv_test

obj-m	+= hello_drv.o