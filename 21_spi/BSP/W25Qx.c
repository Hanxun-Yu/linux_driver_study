//
// Created by xunxun on 22-12-12.
//

/*********************************************************************************************************
*
* File                : ws_W25Qx.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  :
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include <unistd.h>
#include "W25Qx.h"
#include "linux/spi/spidev.h"
#include "stdio.h"

/**
  * @brief  Initializes the W25Q128FV interface.
  * @retval None
  */
uint8_t BSP_W25Qx_Init(int fd_spi) {
    /* Reset W25Qxxx */
    u8 init_ret = BSP_W25Qx_Reset(fd_spi);
    u8 getstatus_ret = BSP_W25Qx_GetStatus(fd_spi);

    u8 ret = init_ret | getstatus_ret;

    printf("BSP_W25Qx_Init ret:%u\n", ret);
    return ret;
}

/**
  * @brief  This function reset the W25Qx.
  * @retval None
  */
static uint8_t BSP_W25Qx_Reset(int fd_spi) {
    uint8_t cmd[2] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};
    u8 ret = HAL_SPI_Transmit_Receive(fd_spi, cmd, 2, NULL, 0);
    /* Send the reset command */
    printf("BSP_W25Qx_Reset ret:%u\n", ret);

    return ret;
}

/**
  * @brief  Reads current status of the W25Q128FV.
  * @retval W25Q128FV memory status
  */
static uint8_t BSP_W25Qx_GetStatus(int fd_spi) {
    printf("BSP_W25Qx_GetStatus start!\n");

    uint8_t cmd[] = {READ_STATUS_REG1_CMD};
    uint8_t receiveData[2] = {0};

    uint8_t status = 0;

    HAL_SPI_Transmit_Receive(fd_spi, cmd, 1, receiveData, 2);


    u32 ret = 0;
    /* Check the value of the register */
    if ((receiveData[1] & W25Q128FV_FSR_BUSY) != 0) {
        ret = W25Qx_BUSY;
    } else {
        ret = W25Qx_OK;
    }
    printf("BSP_W25Qx_GetStatus ret:%u\n", ret);
    return ret;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t BSP_W25Qx_WriteEnable(int fd_spi) {
    printf("BSP_W25Qx_WriteEnable\n");
    uint8_t cmd[] = {WRITE_ENABLE_CMD};
    uint32_t tickstart = HAL_GetTick();

    /* Send the read ID command */
    HAL_SPI_Transmit_Receive(fd_spi, cmd, 1, NULL, 0);

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus(fd_spi) == W25Qx_BUSY);
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE) {
            return W25Qx_TIMEOUT;
        }
    }

    return W25Qx_OK;
}

/**
  * @brief  Read Manufacture/Device ID.
	* @param  return value address
  * @retval None
  */
void BSP_W25Qx_Read_ID(int fd_spi, uint8_t *ID) {
    /**
     *  #define READ_ID_CMD                          0x90
        #define DUAL_READ_ID_CMD                     0x92
        #define QUAD_READ_ID_CMD                     0x94
        #define READ_JEDEC_ID_CMD                    0x9F
     */
    uint8_t cmd[4] = {READ_ID_CMD, 0x00, 0x00, 0x00};
    uint8_t receiveData[8] = {0};

    /* Send the read ID command */
//    HAL_SPI_Transmit(fd_spi, cmd, 4);
    /* Reception of the data */
//    HAL_SPI_Receive(fd_spi, ID, 2);

    HAL_SPI_Transmit_Receive(fd_spi, cmd, 4, receiveData, 8);
    ID[0] = receiveData[5];
    ID[1] = receiveData[6];
}

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Read(int fd_spi, uint8_t *pData, uint32_t ReadAddr, uint32_t Size) {
    uint8_t cmd[4];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t) (ReadAddr >> 16);
    cmd[2] = (uint8_t) (ReadAddr >> 8);
    cmd[3] = (uint8_t) (ReadAddr);

    u8 receiveData[10] = {0};

    /* Reception of the data */
    if (HAL_SPI_Transmit_Receive(fd_spi, cmd, sizeof cmd, receiveData, sizeof receiveData) != HAL_OK) {
        return W25Qx_ERROR;
    }

    int i;
    for (i = 0; i < Size; i++) {
        pData[i] = receiveData[i + 4];
    }
    return W25Qx_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write,No more than 256byte.
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Write(int fd_spi, uint8_t *pData, uint32_t WriteAddr, uint32_t Size) {
    uint8_t cmd[100];
    uint32_t end_addr, current_size, current_addr;
    uint32_t tickstart = HAL_GetTick();

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= WriteAddr) {
        current_addr += W25Q128FV_PAGE_SIZE;
    }
    current_size = current_addr - WriteAddr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size) {
        current_size = Size;
    }

    /* Initialize the adress variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* Perform the write page by page */
    do {
        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t) (current_addr >> 16);
        cmd[2] = (uint8_t) (current_addr >> 8);
        cmd[3] = (uint8_t) (current_addr);

        int i;
        for(i=0;i<Size;i++) {
            cmd[i+4] = pData[i];
        }

        /* Enable write operations */
        BSP_W25Qx_WriteEnable(fd_spi);

        /* Send the command */
        if (HAL_SPI_Transmit_Receive(fd_spi, cmd, 4+Size, NULL, 0) != HAL_OK) {
            return W25Qx_ERROR;
        }

        /* Wait the end of Flash writing */
        while (BSP_W25Qx_GetStatus(fd_spi) == W25Qx_BUSY);
        {
            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE) {
                return W25Qx_TIMEOUT;
            }
        }

        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr)
                                                                         : W25Q128FV_PAGE_SIZE;
    } while (current_addr < end_addr);


    return W25Qx_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Block(int fd_spi, uint32_t Address) {
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t) (Address >> 16);
    cmd[2] = (uint8_t) (Address >> 8);
    cmd[3] = (uint8_t) (Address);

    /* Enable write operations */
    BSP_W25Qx_WriteEnable(fd_spi);

    /* Send the read ID command */
    HAL_SPI_Transmit_Receive(fd_spi, cmd, 4, NULL, 0);
    /*Deselect the FLASH: Chip Select high */

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus(fd_spi) == W25Qx_BUSY);
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME) {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}

/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Chip(int fd_spi) {
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;

    /* Enable write operations */
    BSP_W25Qx_WriteEnable(fd_spi);

    /* Send the read ID command */
    HAL_SPI_Transmit_Receive(fd_spi, cmd, 1, NULL, 0);

    /* Wait the end of Flash writing */
    while (BSP_W25Qx_GetStatus(fd_spi) != W25Qx_BUSY);
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > W25Q128FV_BULK_ERASE_MAX_TIME) {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}


