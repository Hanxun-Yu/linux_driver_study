#### linux 驱动框架
![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220118115546.png)

![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220118120636.png)


#### hello_drv 驱动
* 如何写驱动看hello_drv.c中的注释
* hello_dev_test.c来调用驱动

#### hello_drv 装载驱动
* make编译驱动 生成hello_drv.ko
* 拷贝到开发板
* 开发板装载驱动
  * `insmod hello_drv.ko`
  * `cat /proc/devices` #查看列表中是否存在hello
  * `lsmod` #查看内核中已加载的驱动程序
  * `ls /dev/hello -l` #查看设备节点 

#### hello_drv 测试驱动
* `./hello_drv_test -w yuhanxun`
* `./hello_drv_test -r` #将读出yuhanxun

#### hello_drv 卸载驱动
* `rmmod hello_drv`

#### 查看驱动打印 printk
* 正常控制台输出的是printf
* 执行dmesg来输出printk的打印