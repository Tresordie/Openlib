/*************************************************************************************
 * Description  :
 * Version      : 1.0
 * Author       : simon.yuan
 * Date         : 2018-10-03 11:34:14
 * LastEditors  : simon.yuan
 * LastEditTime : 2021-10-28 17:20:20
 * FilePath     : \\RingBuffer\\ringbuffer.c
 *************************************************************************************/
#include "ringbuffer.h"
#include "stdio.h"
#include "string.h"

unsigned char g_txbuffer[TXBUFFER_SIZE];
unsigned char g_rxbuffer[RXBUFFER_SIZE];
ringbuffer_t  g_ringbuffer;

/*************************************************************************************
 * func:                            ringbuffer_Init
 * brief:                           Init ring buffer
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * param {unsigned char} *pTxbuff   Tx buffer pointer
 * param {unsigned char} *pRxbuff   Rx buffer pointer
 * param {pFunc} pSendOver          Send over callback
 * param {pFuncParam} pRecvNew      Received new byte callback
 * return {*}                       -1: ring buffer non-exist; 0: success to init ring buffer
 *************************************************************************************/
int ringbuffer_Init(ringbuffer_t *pRb, unsigned char *pTxbuff, unsigned char *pRxbuff, pFunc pSendOver, pFuncParam pRecvNew)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        pRb->pTxBuffer    = pTxbuff;
        pRb->TxBufferSize = TXBUFFER_SIZE;
        pRb->TxWriteIndex = 0;
        pRb->TxReadIndex  = 0;
        pRb->TxCounter    = 0;

        pRb->pRxBuffer    = pRxbuff;
        pRb->RxBufferSize = RXBUFFER_SIZE;
        pRb->RxWriteIndex = 0;
        pRb->RxReadIndex  = 0;
        pRb->RxCounter    = 0;

        pRb->pFSendOver   = pSendOver;
        pRb->pFRecvNew    = pRecvNew;
        ret               = 0;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_getRxBuffSize
 * brief:                           get rx ring buffer size
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       >0: buffer size; 0: ring buffer not exist
 *************************************************************************************/
unsigned int ringbuffer_getRxBuffSize(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->RxBufferSize;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_getTxBuffSize
 * brief:                           get tx ring buffer size
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       >0: buffer size; 0: ring buffer not exist
 *************************************************************************************/
unsigned int ringbuffer_getTxBuffSize(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->TxBufferSize;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_resetTxBuffer
 * brief:                           reset tx ring buffer to initial state
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to reset ring buffer; -1: ring buffer not exist
 *************************************************************************************/
int ringbuffer_resetTxBuffer(ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        pRb->TxReadIndex  = 0;
        pRb->TxWriteIndex = 0;
        pRb->TxCounter    = 0;

        ret = 0;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_resetRxBuffer
 * brief:                           reset rx ring buffer to initial state
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to reset ring buffer; -1: ring buffer not exist
 *************************************************************************************/
int ringbuffer_resetRxBuffer(ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        pRb->RxReadIndex  = 0;
        pRb->RxWriteIndex = 0;
        pRb->RxCounter    = 0;

        ret = 0;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_FreepRxBuffer
 * brief:                           free rx buffer which ring buffer pointer points to
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to free ring buffer; -1: ring buffer not exist/error
 *************************************************************************************/
int ringbuffer_FreepRxBuffer(ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        if (pRb->pRxBuffer) {
            pRb->pRxBuffer = NULL;
            ret = 0;
        } else {
            ret = -1;
        }
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_FreepTxBuffer
 * brief:                           free tx buffer which ring buffer pointer points to
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to free ring buffer; -1: ring buffer not exist/error
 *************************************************************************************/
int ringbuffer_FreepTxBuffer(ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        if (pRb->pTxBuffer) {
            pRb->pTxBuffer = NULL;
            ret = 0;
        } else {
            ret = -1;
        }
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_TxBufferCapacity
 * brief:                           ring tx buffer size
 * param {ringbuffer_t} *pRb        ring buffer struct pointer
 * return {*}                       >0: ring buffer capacity; 0: ring buffer not exist/error
 *************************************************************************************/
unsigned int ringbuffer_TxBufferCapacity(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->TxBufferSize;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_RxBufferCapacity
 * brief:                           ring rx buffer size
 * param {ringbuffer_t} *pRb        ring buffer struct pointer
 * return {*}                       >0: ring buffer capacity; 0: ring buffer not exist/error
 *************************************************************************************/
unsigned int ringbuffer_RxBufferCapacity(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->RxBufferSize;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_TxBufferEnd
 * brief:                           get address of ring tx buffer's last byte
 * param {ringbuffer_t} *pRb        pointer to ringbuffer
 * return {*}                       pointer to ring buffer end
 *************************************************************************************/
unsigned char *ringbuffer_TxBufferEnd(const ringbuffer_t *pRb)
{
    unsigned char *pRet = NULL;

    if (NULL == pRb) {
        pRet = NULL;
    } else {
        pRet = (pRb->pTxBuffer + pRb->TxBufferSize - 1);
    }

    return pRet;
}

/*************************************************************************************
 * func:                            ringbuffer_RxBufferEnd
 * brief:                           get address of ring rx buffer's last byte
 * param {ringbuffer_t} *pRb        pointer to ringbuffer
 * return {*}                       pointer to ring buffer end
 *************************************************************************************/
unsigned char *ringbuffer_RxBufferEnd(const ringbuffer_t *pRb)
{
    unsigned char *pRet = NULL;

    if (NULL == pRb) {
        pRet = NULL;
    } else {
        pRet = (pRb->pRxBuffer + pRb->RxBufferSize - 1);
    }

    return pRet;
}

/*************************************************************************************
 * func:                            ringbuffer_TxBufferFreeBytes
 * brief:                           get free size of ring tx buffer
 * param {ringbuf_t} *rb            pointer to ringbuffer
 * return {*}                       -1: ring buff had been overwrited; >0: free size number
 *************************************************************************************/
int ringbuffer_TxBufferFreeBytes(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->TxCounter <= pRb->TxBufferSize) {
        ret = pRb->TxBufferSize - pRb->TxCounter;
    } else {
        // overwrite ring buffer
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_RxBufferFreeBytes
 * brief:                           get free size of ring rx buffer
 * param {ringbuf_t} *rb            pointer to ringbuffer
 * return {*}                       -1: ring buff had been overwrited; >0: free size number
 *************************************************************************************/
int ringbuffer_RxBufferFreeBytes(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->RxCounter <= pRb->RxBufferSize) {
        ret = pRb->RxBufferSize - pRb->RxCounter;
    } else {
        // overwrite ring buffer
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_TxBufferBytesUsed
 * brief:                           get bytes used in ring tx buffer
 * param {*}                        pointer to ringbuffer
 * return {*}                       used buffer size
 *************************************************************************************/
unsigned int ringbuffer_TxBufferBytesUsed(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->TxCounter;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_RxBufferBytesUsed
 * brief:                           get bytes used in ring rx buffer
 * param {*}                        pointer to ringbuffer
 * return {*}                       used buffer size
 *************************************************************************************/
unsigned int ringbuffer_RxBufferBytesUsed(const ringbuffer_t *pRb)
{
    unsigned int ret = 0;

    if (NULL == pRb) {
        ret = 0;
    } else {
        ret = pRb->RxCounter;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_IsTxBufferFull
 * brief:                           check if ring tx buffer full or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: full; -1: not full
 *************************************************************************************/
int ringbuffer_IsTxBufferFull(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->TxCounter >= pRb->TxBufferSize) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuffer_IsRxBufferFull
 * brief:                           check if ring rx buffer full or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: full; -1: not full
 *************************************************************************************/
int ringbuffer_IsRxBufferFull(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->RxCounter >= pRb->RxBufferSize) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuf_IsTxBufferEmpty
 * brief:                           check if ring tx buffer empty or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: empty; -1: not empty
 *************************************************************************************/
int ringbuf_IsTxBufferEmpty(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->TxCounter == 0) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                            ringbuf_IsRxBufferEmpty
 * brief:                           check if ring rx buffer empty or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: empty; -1: not empty
 *************************************************************************************/
int ringbuf_IsRxBufferEmpty(const ringbuffer_t *pRb)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else if (pRb->RxCounter == 0) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferWriteIn
 * brief:                                   write in data into rx ring buffer
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *dat               pointer to data which will be wrote into ring buffer
 * return {*}                               0: success to write in; -1: ring buffer pointer not exist/error
 *************************************************************************************/
int ringbuffer_RxBufferWriteIn(ringbuffer_t *pRb, unsigned char *dat)
{
    int ret = -1;

    if (NULL == pRb) {
        ret = -1;
    } else {
        *(pRb->pRxBuffer + pRb->RxWriteIndex) = *dat;       // overwrite ring buffer if data length more than buffer size
        if (pRb->RxWriteIndex >= (pRb->RxBufferSize - 1)) {
            pRb->RxWriteIndex = 0;
        } else {
            (pRb->RxWriteIndex)++;
        }

        /* overwrite happened, move forware RxReadIndex, RxCounter always less than buffer size */
        (pRb->RxCounter)++;
        if (pRb->RxCounter > pRb->RxBufferSize) {
            pRb->RxReadIndex = pRb->RxWriteIndex;
            pRb->RxCounter   = pRb->RxBufferSize;
        }
        ret = 0;
    }

    return ret;
}

/*************************************************************************************
 * func:                                    ringbuffer_FindCharInRxBuffer
 * brief:                                   find specified character in ring rx buffer
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *dat               specified char
 * return {*}                               -1: not found;  >0: locationIndex
 *************************************************************************************/
int ringbuffer_FindCharInRxBuffer(ringbuffer_t *pRb, unsigned char *dat)
{
    unsigned int index = 0;
    unsigned char temp = 0;
    int locationIndex  = -1;

    if (NULL == pRb) {
        locationIndex = -1;
    } else {
        if ((pRb->RxCounter > 0) && (pRb->RxCounter <= pRb->RxBufferSize)) {   /* have rx counter data */
            if (pRb->RxWriteIndex > pRb->RxReadIndex) {
                for (index = pRb->RxReadIndex; index < pRb->RxWriteIndex; index++) {
                    temp = pRb->pRxBuffer[index];
                    (pRb->RxReadIndex)++;
                    (pRb->RxCounter)--;
                    if ((*dat) == temp) {
                        locationIndex = index;
                        return locationIndex;
                    }
                }

                /* Not found */
                if (index == pRb->RxWriteIndex) {
                    locationIndex    = -1;
                    pRb->RxCounter   = 0;
                    pRb->RxReadIndex = pRb->RxWriteIndex;
                }
            } else {
                // RxReadIndex -> RxBufferSize & 0 -> RxWriteIndex
                for (index = pRb->RxReadIndex; index < pRb->RxBufferSize; index++) {
                    temp = pRb->pRxBuffer[index];
                    (pRb->RxCounter)--;
                    if ((*dat) == temp) {
                        pRb->RxReadIndex = index;  // found specified char, read index start
                        locationIndex    = index;
                        return locationIndex;
                    }
                }
                if (pRb->RxBufferSize == index)
                    locationIndex = -1;

                for (index = 0; index < pRb->RxWriteIndex; index++) {
                    temp = pRb->pRxBuffer[index];
                    (pRb->RxCounter)--;
                    if ((*dat) == temp) {
                        pRb->RxReadIndex = index;  // found specified char, read index start
                        locationIndex    = index;
                        return locationIndex;
                    }
                }
                if (pRb->RxWriteIndex == index)
                    locationIndex = -1;
            }
        } else if (pRb->RxCounter == 0) {
            locationIndex = -1;
        }
    }

    return locationIndex;
}

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferReadOut
 * brief:                                   read out size data into pBuf
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *pBuf              pointer to data which will be wrote into ring buffer
 * param {unsigned int} size                how many bytes been read out
 * return {*}                               0: success to write in; -1: ring buffer pointer not exist/error
 *************************************************************************************/
int ringbuffer_RxBufferReadOut(ringbuffer_t *pRb, unsigned char *pBuf, unsigned int size)
{
    int ret = -1;
    unsigned int i = 0;

    if (NULL == pRb) {
        ret = -1;
    } else {
        if (0 == (pRb->RxCounter)) {
            ret = -1;
        } else if ((pRb->RxCounter) && (pRb->RxCounter < size)) {
            ret = -1;
        } else if (pRb->RxCounter >= size) {
            for (i = 0; i < size; i++) {
                pBuf[i] = pRb->pRxBuffer[pRb->RxReadIndex];
                (pRb->RxReadIndex)++;
                (pRb->RxCounter)--;
                if (pRb->RxReadIndex >= pRb->RxBufferSize) {
                    pRb->RxReadIndex = 0;
                }
            }
            ret = 0;
        }
    }

    return ret;
}

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferMemberPrint
 * brief:                                   printf rx buffer member
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * return {*}                               None
 *************************************************************************************/
void ringbuffer_RxBufferMemberPrint(ringbuffer_t *pRb)
{
    printf("pRb->RxBufferSize = %d\r\n", pRb->RxBufferSize);
    printf("pRb->RxWriteIndex = %d\r\n", pRb->RxWriteIndex);
    printf("pRb->RxReadIndex  = %d\r\n", pRb->RxReadIndex);
    printf("pRb->RxCounter    = %d\r\n", pRb->RxCounter);
}

/*************************************************************************************
 * func:                                    ringbuffer_TxBufferMemberPrint
 * brief:                                   printf tx buffer member
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * return {*}                               None
 *************************************************************************************/
void ringbuffer_TxBufferMemberPrint(ringbuffer_t *pRb)
{
    printf("pRb->TxBufferSize = %d\r\n", pRb->TxBufferSize);
    printf("pRb->TxWriteIndex = %d\r\n", pRb->TxWriteIndex);
    printf("pRb->TxReadIndex  = %d\r\n", pRb->TxReadIndex);
    printf("pRb->TxCounter    = %d\r\n", pRb->TxCounter);
}
