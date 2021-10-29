#include "bsp.h"
#include "main.h"
#include "sled.h"


// define 2 single indicator led nodes
LedNode_t *lednode1 = NULL;
LedNode_t *lednode2 = NULL;

// function declare
static void PrintfLogo(void);
void sled_action(sled_t *sled);

// Initialized default sled data
sled_t sled1 = {
    "led1",
    1,
    S_GREEN,
    LED_FLASH,
    400,
};

sled_t sled2 = {
    "led2",
    2,
    S_GREEN,
    LED_FLASH,
    200,
};

int main(void)
{
    SysTick_Config(SystemCoreClock / 1000);

    bsp_Init();

    PrintfLogo();

    // create sled node and add into led list
    sledlist_init();
    
    sled_create(lednode1, &sled1);

    sled_create(lednode1, &sled2);

    while (1)
    {
        // sled polling process
        sled_cycle_process();
    }
}


/*
*********************************************************************************************************
*   函 数 名: PrintfLogo
*   功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    printf("*************************************************************\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}


