#### gpio 硬件框架
![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119155322.png)
通过配置寄存器来实现上图的配置

#### 寄存器操作
1. 读出-修改-写入
   ```c
   //置1第n个bit
   val = data_reg;
   val = val | (1<<n);
   data_reg = val;

   //置0第n个bit
   val = data_reg;
   val = val & ~(1<<n);
   data_reg = val;
   ```
2. set-and-clear 方式  
   set_reg, clr_reg, data_reg 三个寄存器对应的是同一个物理寄存器  
   置1  第n个bit：`set_reg = (1<<n);`  
   置0  第n个bit：`clr_reg = (1<<n);`

