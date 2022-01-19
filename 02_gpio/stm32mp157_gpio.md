#### 157 gpio操作步骤
* 使能时钟
  * 157与其他开发板相比，需多一步，使能时钟  
  * 搜索芯片手册 10.7.43
  * PLL4 寄存器
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171214.png)
  寄存器基址：0x50000000
* 使能gpio模块
  * 搜索芯片手册 10.7.158
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171248.png)
* 设置引脚工作模式
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171334.png)
* 设置输出模式（若引脚为输出的话）
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171414.png)
* 设置输出速度
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171443.png)
  * 输出速度高，上升沿或下降沿经历的时间就短，波形更陡
  * 输出速度高，对周围电路影响越大
* 设置上下拉电阻
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171559.png)
* 读取引脚电平
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171629.png)
* 设置引脚电平
  * 方法一
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171654.png)
  * 方法二
  ![](https://picbed-xunxun.oss-cn-shanghai.aliyuncs.com/20220119171721.png)
