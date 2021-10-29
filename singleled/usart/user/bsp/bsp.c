#include "bsp.h"


static void bsp_led_gpioinit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   
    RCC_APB2PeriphClockCmd(LEDGP1_RCC|LEDGP2_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED1_PIN|LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDGP1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED3_PIN|LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDGP2_PORT, &GPIO_InitStructure);

    GPIO_SetBits(LEDGP1_PORT, LED1_PIN);
    GPIO_SetBits(LEDGP1_PORT, LED2_PIN);
    GPIO_SetBits(LEDGP2_PORT, LED3_PIN);
    GPIO_SetBits(LEDGP2_PORT, LED4_PIN);
}


static void bsp_InitUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_IDLE|USART_IT_RXNE|USART_IT_TC|USART_IT_TXE);
}


int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return (ch);
}



/*
*********************************************************************************************************
*   �� �� ��: bsp_Init
*   ����˵��: ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
*            ȫ�ֱ�����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    bsp_led_gpioinit();
    bsp_InitUart();
}




