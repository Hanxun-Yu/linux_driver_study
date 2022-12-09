### SPI

这里着急先把任务实现，笔记回头补起来，先简单写一些  

spi驱动分为2部分，控制器驱动、设备驱动  
spi控制器可以有两种模式，主模式、从模式  


#### SPI协议
spi引脚
* CLK 时钟
* MISO 主输入 从输出
* MOSI 主输出 从输入
* CS 片选
有独立的输入与输出引脚，所以spi具有全双工传输能力    

4种传输模式  
    CPOL 极性（初始时钟状态  高或低）
    CPHA 相位（第1或2个边沿触发数据采集）  
    根据这2个状态，会组合出4种模式，下面放一张图  

##### 数据发送过程
比如需要发送2个字节数据，即16位，u8 data[2]  
发送函数是从data[0]开始发送， 从每个字节最高位开始发送  
这2个字节，总共16位，先发高8位还是低8位，取决与设备协议  





#### SPI控制器内部结构
放一张图  



#### 编译spi_dev.c
源码目录下make menuconfig  
```shell
-> Device Drivers
  -> SPI support (SPI [=y]) 
    < >   User mode SPI device driver support  
```
如果`User mode SPI device driver support`前面不是`<Y>`或`<M>`，可以输入`M`表示把它编译为模块。

* 如果已经是`<Y>`，则不用再做其他事情。
* 如果你设置为`<M>`，在内核目录下执行`make modules`，把生成的`drivers/spi/spidev.ko`复制到NFS目录备用







