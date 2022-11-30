

# 该文件方便这套代码在Clion中使用，不用于实际编译

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(demo)

include_directories(
        .
        inc
        src
        Linux-5.4_link/include
)

AUX_SOURCE_DIRECTORY(01_hello_drv SRC_LIST)
AUX_SOURCE_DIRECTORY(02_gpio SRC_LIST)
AUX_SOURCE_DIRECTORY(03_led_drv SRC_LIST)

ADD_EXECUTABLE(demo ${SRC_LIST})