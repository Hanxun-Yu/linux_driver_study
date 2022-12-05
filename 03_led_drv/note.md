### 实现了led开关驱动，分离驱动注册与硬件相关操作  
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

#### 物理地址-虚拟地址
> 在芯片手册中确定的寄存器地址被称为物理地址，在Linux内核中无法直接使用。
      需要使用内核提供的ioremap把物理地址映射为虚拟地址，使用虚拟地址。

把物理地址phys_addr开始的一段空间(大小为size)，映射为虚拟地址；返回值是该段虚拟地址的首地址。  
`virt_addr  = ioremap(phys_addr, size);`
>实际上，它是按页(4096字节)进行映射的，是整页整页地映射的。
假设phys_addr = 0x10002，size=4，ioremap的内部实现是：
a. phys_addr按页取整，得到地址0x10000
b. size按页取整，得到4096
c. 把起始地址0x10000，大小为4096的这一块物理地址空间，映射到虚拟地址空间，
   假设得到的虚拟空间起始地址为0xf0010000
d. 那么phys_addr = 0x10002对应的virt_addr = 0xf0010002



不再使用该段虚拟地址时，要iounmap(virt_addr)：  
`void iounmap(volatile void __iomem *cookie)`

#### volatile
1. 有时候编译器会自作聪明，比如：
```
int *p = ioremap(xxxx, 4);  // GPIO寄存器的地址
*p = 0;   // 点灯，但是这句话被优化掉了
*p = 1;   // 灭灯
```

2. 对于上面的情况，为了避免编译器自动优化，需要加上volatile，告诉它“这是容易出错的，别乱优化”：
```
volatile  int *p = ioremap(xxxx, 4);  // GPIO寄存器的地址
*p = 0;   // 点灯，这句话不会被优化掉
*p = 1;   // 灭灯
```
