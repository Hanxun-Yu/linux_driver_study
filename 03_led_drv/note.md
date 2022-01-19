以下基于157开发板 

#### 确认硬件连接与对应寄存器参数
1. 看原理图找到需要控制的led （原理图搜索LED2）
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119162846.png)
2. 可以看到由PA10脚控制LED2 (原理图搜索PA10)
   ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119163057.png)
3. PA10，其中A表示属于GPIO A组，10表示该组第10个脚
4. GPIOA使能寄存器（芯片手册搜索GPIOAEN）
   ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119164116.png)
   可以看到该寄存器位于AHB4总线，偏移地址为0xA28
5. 搜索AHB4总线基址
   1. 芯片手册搜索AHB4，会出来一堆
   2. 或者搜索 register boundary address
   ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119164647.png)

#### gpio 操作流程（可以看02_gpio）


#### led 驱动框架
* 大体框架
    ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119173836.png)
* 代码架构（如何方便适配多种开发板）
    ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119173937.png)
    * 方法一，不同开发板写自己的驱动实现，编译对应的实现c文件
    * 方法二，只抽象出函数，不同板写自己的抽象函数实现，编译对应的c文件