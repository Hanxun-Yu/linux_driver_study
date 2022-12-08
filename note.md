#### 链接linux内核源码(链接方式vscode会有问题，还是拷贝吧)
    cd linux_driver_study  
    ln -s Linux-5.4_link <linux_kernel_source_absPath>

#### vscode 包含linux内核头文件
    编辑.vscode/c_cpp_properties.json  
    configurations下的includePath加入"${workspaceFolder}/<linux_kernel_source_link>/include"

#### 交叉编译 环境变量
    export ARCH=arm
    export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
    export PATH=$PATH:/home/${USER}/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin`
    
    可以把上面写入脚本cross_env.sh  
    source ~/100ask_stm32mp157_pro-sdk/cross_env.sh

    编译eg:  
    arm-buildroot-linux-gnueabihf-gcc helloworld.c -o helloworld



#### board 配置网络（确认网口位置：开关在左下角的时，左eth1，右eth0）
    静态ip
    ifconfig eth1 10.1.1.251 netmask 255.255.255.0
    ifconfig eth1 down
    ifconfig eth1 up

    #dhcp获取ip
    udhcpc -i eth1


#### 文件共享

    （共享目录必须使用nfs，因为编译结果里存在文件链接）
    
    #board mount ubuntu
    mount -t nfs -o nolock,vers=3 10.1.1.114:/home/yuhanxun/nfs_share /mnt/nfs_share

    #pc mount ubuntu
    sudo mount -t nfs -o nolock,vers=3 10.1.1.114:/home/yuhanxun/nfs_share /mnt/nfs_share

    #编译结果放到共享目录
    rm -rf /mnt/nfs_share/wds/linux_driver_study
    cp -r ../linux_driver_study /mnt/nfs_share/wds/linux_driver_study

#### 开发板自动mount 修改/etc/profile
```
加入
mkdir /mnt/nfs_share
ifconfig eth1 10.1.1.251 netmask 255.255.255.0
sleep 1
mount -t nfs -o nolock,vers=3 10.1.1.114:/home/yuhanxun/nfs_share /mnt/nfs_share
```


#### stm32mp157 
    #关闭gui
    systemctl stop myir
    #关闭心跳灯 led2
    echo none > /sys/class/leds/heartbeat/trigger


#### 开启gdb调试
    1.板子上执行 ulimit -c unlimited
    2.gcc编译时加上参数 -g
    3.报错时会产生core文件
    执行gdb <program> <corefile>

#### printk信息 打印开关
    #关闭
    echo "1 4 1 7" > /proc/sys/kernel/printk
    #打开
    echo "7 4 1 7" > /proc/sys/kernel/printk