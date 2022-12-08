

/**
 * 提供给button_drv_stm32mp157.c来实现，并注册给button_drv.c
 */
#pragma once


struct button_operations {
    /**
     * 按键数
     */
    int count;

    /**
     * 初始化
     * @param which 按键序号
     */
    void (*init)(int which);


    /**
     * 读取按键状态
     * @param which 按键序号
     * @return
     */
    int (*read)(int which);
};

void register_button_operations(struct button_operations *opr);

void unregister_button_operations(void);





