

# 该文件方便这套代码在Clion中使用，不用于实际编译
PROJECT(07_button_drv)


# 定义这个宏，才可用ide跟踪到一些源码(一些源码只对内核编译开发 比如ioremap)
add_definitions(-D__KERNEL__)

include_directories(
        .
        inc
        src
        Linux-5.4_link/include
        Linux-5.4_link/arch/arm/include
        ~/work/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
)

AUX_SOURCE_DIRECTORY(src SRC_LIST)

ADD_EXECUTABLE(07_button_drv ${SRC_LIST})