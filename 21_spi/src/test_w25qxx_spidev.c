//
// Created by xunxun on 22-12-12.
//



#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

//#include <sys/ioctl.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include <linux/spi/spidev.h>
#include <stdbool.h>
#include "app_base_type.h"
#include "W25Qx.h"

/**
 * 使用linux自带spi驱动，spidev方式，只写应用层代码与spi设备通讯
 * 这里使用w25qxx flash设备，具体设备是 w25q128
 *
 * 这个测试程序传入2或3个参数，读取或写入固定4个字节的数据
 *
 * 参数0：操作类型  r/w
 * 参数1：地址  读取地址/写入地址 (不应超过 0x1000000 - 4， flash大小 128 MBits => 16MBytes)
 * 参数2：字符串（4个字母）
 *
 */
int main(int argc, char **argv) {
    bool cmdError = false;
    bool isRead = false;


    char arg_fd_path[100];
    char arg_opr_type[1];
    char arg_addr[10];
    char arg_val[4];


    if (argc == 4) {
        //read
        if (strcmp(argv[2], "r") == 0) {
            isRead = true;
        } else {
            cmdError = true;
        }
    } else if (argc == 5) {
        //write
        if (strcmp(argv[2], "w") == 0) {
            isRead = false;
        } else {
            cmdError = true;
        }
    } else {
        cmdError = true;
    }

    if (cmdError) {
        printf("Read Usage: %s /dev/spidevB.D r <addr>\n", argv[0]);
        printf("Write Usage: %s /dev/spidevB.D w <addr> <str>\n", argv[0]);
        return 0;
    }


    strcpy(arg_fd_path, argv[1]);
    strcpy(arg_opr_type, argv[2]);
    strcpy(arg_addr, argv[3]);
    u32 addr_u32 = strtoul(arg_addr, NULL, 0);


    int fd = open(arg_fd_path, O_RDWR);
    if (fd < 0) {
        printf("can not open %s\n", arg_fd_path);
        return -1;
    }

    if (HAL_SPI_Init(fd) != HAL_OK) {
        printf("HAL_SPI_Init error \n");
        return -1;
    }


    u8 ID[4] = {0};
    BSP_W25Qx_Init(fd);
    BSP_W25Qx_Read_ID(fd, ID);
    printf("W25Qxxx ID is : ");
    int i = 0;
    for (i = 0; i < 2; i++) {
        printf("0x%02X ", ID[i]);
    }

    printf("\n");


    if (isRead) {
        printf("Read addr:0x%04x\n", addr_u32);
        //BSP_W25Qx_Read(int fd_spi, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
        u8 data[2];
        u32 addr = 0x200;
        u8 ret = BSP_W25Qx_Read(fd, data, addr, 2);

        for (i = 0; i < 2; i++) {
            printf("0x%02x\n ", data[i]);
        }

        printf("read ret:%d\n", ret);


    } else {
        printf("Write addr:0x%04x\n", addr_u32);
        strcpy(arg_val, argv[4]);

        u8 data[2];
        data[0] = 0x03;
        data[1] = 0x22;
        u32 addr = 0x200;
        u8 ret = BSP_W25Qx_Write(fd, data, addr, 2);
        printf("write ret:%d\n", ret);
    }


    return 0;
    struct spi_ioc_transfer xfer[1];
    int ioctl_status;



//
//    val = strtoul(argv[2], NULL, 0);
//    val <<= 2;     /* bit0,bit1 = 0b00 */
//    val &= 0xFFC;  /* 只保留10bit */
//
//    tx_buf[1] = val & 0xff;
//    tx_buf[0] = (val >> 8) & 0xff;
//
//    memset(xfer, 0, sizeof xfer);
//
//
//    xfer[0].tx_buf = tx_buf;
//    xfer[0].rx_buf = rx_buf;
//    xfer[0].len = 2;
//
//    status = ioctl(fd, SPI_IOC_MESSAGE(1), xfer);
//    if (status < 0) {
//        printf("SPI_IOC_MESSAGE\n");
//        return -1;
//    }
//
//    /* 打印 */
//    val = (rx_buf[0] << 8) | (rx_buf[1]);
//    val >>= 2;
//    printf("Pre val = %d\n", val);


    return 0;
}
