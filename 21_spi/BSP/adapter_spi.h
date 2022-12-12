//
// Created by xunxun on 22-12-12.
//

#pragma once

#include "adapter_type.h"

typedef enum {
    HAL_OK = 0x00,
    HAL_ERROR = 0x01,
//    HAL_BUSY = 0x02,
//    HAL_TIMEOUT = 0x03
} HAL_StatusTypeDef;


uint32_t HAL_GetTick(void);


HAL_StatusTypeDef HAL_SPI_Init(int fd_spi);
HAL_StatusTypeDef HAL_SPI_Transmit(int fd_spi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive(int fd_spi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Transmit_Receive(int fd_spi, uint8_t *wData, uint8_t *rData, uint16_t Size);

