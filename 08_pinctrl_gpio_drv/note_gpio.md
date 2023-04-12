### GPIO
#### 意义
使用GPIO之前，先要通过Pinctrl来将引脚选择为GPIO功能，然后才能控制GPIO  
GPIO可以有3种控制操作：
* 引脚方向
* 读值-获得电平状态
* 写值-输出高低电平

以前操作GPIO，需要控制对应寄存器，每块板子都不一样  
现在芯片厂商的BSP工程师实现了GPIO子系统，我们可以在设备树中指定GPIO引脚  
然后在驱动中**使用GPIO子系统的标准函数来控制GPIO，这种代码与板子解耦**  
<br>


#### 设备树中指定引脚 15.2.2
![20230412153243.png](pic%2F20230412153243.png)  
BSP工程师会在设备树中先定义好所有可用的GPIO组，即GPIO Controller  
图中**gpio-controller;**  表示这个节点是一个GPIO Controller    
图中#gpio-cells=<2>表示这个控制器下每一个引脚要用 2 个 32 位的数(cell)来描述  
<br>
<br>
![20230412154318.png](pic%2F20230412154318.png)  
上面GPIO Controller中标识的#gpio-cells=<2>，表示使用2个cell  
用第 1 个 cell 来表示哪一个引脚，用第 2 个 cell 来表示有效电平    
GPIO_ACTIVE_HIGH ： 高电平有效  
GPIO_ACTIVE_LOW : 低电平有效  
*这样一来，代码中只需要控制（有效、无效），不需要具体控制（High、Low）*
