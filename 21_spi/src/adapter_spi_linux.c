//
// Created by xunxun on 22-12-12.
//
#include "adapter_spi.h"
#include <sys/time.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <stdbool.h>

/**
 * 针对adapter_spi.h的linux spi实现
 */

HAL_StatusTypeDef HAL_SPI_Init(int fd_spi) {
    //设置mode
    u32 mode = 0;
    int status = ioctl(fd_spi, SPI_IOC_RD_MODE, &mode);
    if (status < 0) {
        return HAL_ERROR;
    }
    printf("get mode:%x\n", mode);

    mode = mode | SPI_MODE_3 | SPI_LSB_FIRST ;

    printf("set mode:%x\n", mode);

    status = ioctl(fd_spi, SPI_IOC_WR_MODE, &mode);
    if (status < 0) {
        return HAL_ERROR;
    }

    if (status < 0) {
        return HAL_ERROR;
    }
    return HAL_OK;
}


uint32_t HAL_GetTick(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

HAL_StatusTypeDef HAL_SPI_Transmit(int fd_spi, uint8_t *pData, uint16_t Size) {
    struct spi_ioc_transfer xfer[1];
    memset(xfer, 0, sizeof xfer);
    xfer[0].tx_buf = (__u64) pData;
    xfer[0].len = Size;
    xfer[0].speed_hz = 104000000;
    xfer[0].bits_per_word = 8;
    int status = ioctl(fd_spi, SPI_IOC_MESSAGE(1), xfer);
//    printf("HAL_SPI_Transmit ioctl status:%d\n", status);

    u32 ret = status >= 0 ? HAL_OK : HAL_ERROR;
    printf("HAL_SPI_Transmit ret:%d\n", ret);

    return ret;
}

HAL_StatusTypeDef HAL_SPI_Receive(int fd_spi, uint8_t *pData, uint16_t Size) {
    struct spi_ioc_transfer xfer[1];
    memset(xfer, 0, sizeof xfer);
    xfer[0].rx_buf = pData;
    xfer[0].tx_buf = 0;
    xfer[0].len = Size;
    xfer[0].speed_hz = 104000000;
    xfer[0].bits_per_word = 8;
    int status = ioctl(fd_spi, SPI_IOC_MESSAGE(1), xfer);
    printf("HAL_SPI_Receive ioctl status:%d\n", status);
    int i = 0;
    for (i = 0; i < Size; i++) {
        printf("HAL_SPI_Receive read:%02x\n", pData[i]);
    }
    return status >= 0 ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef HAL_SPI_Transmit_Receive(int fd_spi, uint8_t *wData, uint8_t *rData, uint16_t Size) {
    struct spi_ioc_transfer xfer[1];
    memset(xfer, 0, sizeof xfer);
    xfer[0].tx_buf =  wData;
    xfer[0].rx_buf =  rData;
    xfer[0].len = Size;
    xfer[0].speed_hz = 104000000;
    xfer[0].bits_per_word = 8;
    int status = ioctl(fd_spi, SPI_IOC_MESSAGE(1), xfer);
    printf("HAL_SPI_Transmit_Receive ioctl status:%d\n", status);

    int i = 0;
    for (i = 0; i < Size; i++) {
        printf("HAL_SPI_Transmit_Receive read:%02x\n", rData[i]);
    }

    u32 ret = status >= 0 ? HAL_OK : HAL_ERROR;
    printf("HAL_SPI_Transmit ret:%d\n", ret);

    return ret;
}
