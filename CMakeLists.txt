PROJECT(demo)

# 该文件方便这套代码在Clion中使用，不用于实际编译

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


# 定义这个宏，才可用ide跟踪到一些源码(一些源码只对内核编译开发 比如ioremap)
add_definitions(-D__KERNEL__)
include_directories(
        Linux-5.4_link/include
        Linux-5.4_link/arch/arm/include
        ~/work/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
)

#AUX_SOURCE_DIRECTORY(01_hello_drv SRC_LIST)
#AUX_SOURCE_DIRECTORY(02_gpio SRC_LIST)
#AUX_SOURCE_DIRECTORY(03_led_drv SRC_LIST)
#AUX_SOURCE_DIRECTORY(04_led_drv_separate SRC_LIST)

# 因为上面的方式会导致代码追踪跨项目目录，所以用下面的方式
# 在每个子目录里有自己的CMakeLists，作为每个子项目
add_subdirectory(05_led_drv_bus)
add_subdirectory(06_led_drv_device_tree)
