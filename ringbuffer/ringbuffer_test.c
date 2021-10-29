/*************************************************************************************
 * Description  : Test code for ring buffer library
 * Version      : 1.0
 * Author       : simon.yuan
 * Date         : 2018-10-03 11:34:14
 * LastEditors  : simon.yuan
 * LastEditTime : 2021-10-28 20:13:08
 * FilePath     : \\RingBuffer\\ringbuffer_test.c
 *************************************************************************************/

#include "stdio.h"
#include "ringbuffer.h"

#define  RINGBUFFER_WRITEIN_SIZE            10
#define  RINGBUFFER_READOUT_SIZE            12

void SendOver(void)
{
    printf("%s\r\n", __func__);
}

void RecvNew(unsigned char *recvNew)
{
    printf("%s\r\n", __func__);
}


int main(void)
{
    static unsigned char fchar = 7;
    unsigned char i = 0;
    unsigned char pBuf[RINGBUFFER_READOUT_SIZE] = {0};
    int temp = -1;

    /* ring buffer init */
    ringbuffer_Init(&g_ringbuffer, g_txbuffer, g_rxbuffer, SendOver, RecvNew);

    /* rx ring buffer write in */
    for (i = 0; i < RINGBUFFER_WRITEIN_SIZE; i++) {
        if (ringbuffer_RxBufferWriteIn(&g_ringbuffer, &i) < 0) {
            break;
        }
    }
    ringbuffer_RxBufferMemberPrint(&g_ringbuffer);

    /* rx ring buffer member printf */
    for (i = 0; i < RXBUFFER_SIZE; i++) {
        printf("g_ringbuffer.pRxBuffer[%d] = %d\r\n", i, g_ringbuffer.pRxBuffer[i]);
    }

    /* find char in rx ring buffer */
    // temp = ringbuffer_FindCharInRxBuffer(&g_ringbuffer, &fchar);
    // if (temp >= 0) {
    //     printf("find fchar at %d\r\n", temp);
    // } else {
    //     printf("Not find fchar\r\n");
    // }

    /* rx ring buffer read out */
    if (0 == ringbuffer_RxBufferReadOut(&g_ringbuffer, pBuf, RINGBUFFER_READOUT_SIZE)) {
        printf("ringbuffer_RxBufferReadOut succeeded\r\n");
    } else {
        printf("ringbuffer_RxBufferReadOut error\r\n");
    }
    for (i = 0; i < RINGBUFFER_READOUT_SIZE; i++) {
        printf("pBuf[%d] = %d\r\n", i, pBuf[i]);
    }
    ringbuffer_RxBufferMemberPrint(&g_ringbuffer);

    return 0;
}