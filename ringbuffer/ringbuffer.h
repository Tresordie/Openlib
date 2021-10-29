/*************************************************************************************
 * Description  :
 * Version      : 1.0
 * Author       : simon.yuan
 * Date         : 2018-10-03 11:34:14
 * LastEditors  : simon.yuan
 * LastEditTime : 2021-10-28 16:55:50
 * FilePath     : \\RingBuffer\\ringbuffer.h
 *************************************************************************************/
#ifndef __RINGBUFFER_H___
#define __RINGBUFFER_H___

#define TXBUFFER_SIZE     10
#define RXBUFFER_SIZE     10

typedef void (*pFunc)(void);
typedef void (*pFuncParam)(unsigned char *dat);

typedef struct _ringbuffer_t {
    unsigned char *pTxBuffer;
    unsigned int  TxReadIndex;
    unsigned int  TxWriteIndex;
    unsigned int  TxBufferSize;
    unsigned int  TxCounter;

    unsigned char *pRxBuffer;
    unsigned int  RxReadIndex;
    unsigned int  RxWriteIndex;
    unsigned int  RxBufferSize;
    unsigned int  RxCounter;

    void (*pFSendOver)(void);
    void (*pFRecvNew)(unsigned char *recvNew);
} ringbuffer_t;

extern unsigned char g_txbuffer[TXBUFFER_SIZE];
extern unsigned char g_rxbuffer[RXBUFFER_SIZE];
extern ringbuffer_t  g_ringbuffer;


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
int ringbuffer_Init(ringbuffer_t *pRb, unsigned char *pTxbuff, unsigned char *pRxbuff, pFunc pSendOver, pFuncParam pRecvNew);

/*************************************************************************************
 * func:                            ringbuffer_getRxBuffSize
 * brief:                           get rx ring buffer size
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       >0: buffer size; 0: ring buffer not exist
 *************************************************************************************/
unsigned int ringbuffer_getRxBuffSize(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_getTxBuffSize
 * brief:                           get tx ring buffer size
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       >0: buffer size; 0: ring buffer not exist
 *************************************************************************************/
unsigned int ringbuffer_getTxBuffSize(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_resetTxBuffer
 * brief:                           reset tx ring buffer to initial state
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to reset ring buffer; -1: ring buffer not exist
 *************************************************************************************/
int ringbuffer_resetTxBuffer(ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_resetRxBuffer
 * brief:                           reset rx ring buffer to initial state
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to reset ring buffer; -1: ring buffer not exist
 *************************************************************************************/
int ringbuffer_resetRxBuffer(ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_FreepRxBuffer
 * brief:                           free rx buffer which ring buffer pointer points to
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to free ring buffer; -1: ring buffer not exist/error
 *************************************************************************************/
int ringbuffer_FreepRxBuffer(ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_FreepTxBuffer
 * brief:                           free tx buffer which ring buffer pointer points to
 * param {ringbuffer_t} *pRb        ring buffer pointer
 * return {*}                       0: success to free ring buffer; -1: ring buffer not exist/error
 *************************************************************************************/
int ringbuffer_FreepTxBuffer(ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_TxBufferCapacity
 * brief:                           ring tx buffer size
 * param {ringbuffer_t} *pRb        ring buffer struct pointer
 * return {*}                       >0: ring buffer capacity; 0: ring buffer not exist/error
 *************************************************************************************/
unsigned int ringbuffer_TxBufferCapacity(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_RxBufferCapacity
 * brief:                           ring rx buffer size
 * param {ringbuffer_t} *pRb        ring buffer struct pointer
 * return {*}                       >0: ring buffer capacity; 0: ring buffer not exist/error
 *************************************************************************************/
unsigned int ringbuffer_RxBufferCapacity(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_TxBufferEnd
 * brief:                           get address of ring tx buffer's last byte
 * param {ringbuffer_t} *pRb        pointer to ringbuffer
 * return {*}                       pointer to ring buffer end
 *************************************************************************************/
unsigned char *ringbuffer_TxBufferEnd(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_RxBufferEnd
 * brief:                           get address of ring rx buffer's last byte
 * param {ringbuffer_t} *pRb        pointer to ringbuffer
 * return {*}                       pointer to ring buffer end
 *************************************************************************************/
unsigned char *ringbuffer_RxBufferEnd(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_TxBufferFreeBytes
 * brief:                           get free size of ring tx buffer
 * param {ringbuf_t} *rb            pointer to ringbuffer
 * return {*}                       -1: ring buff had been overwrited; >0: free size number
 *************************************************************************************/
int ringbuffer_TxBufferFreeBytes(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_RxBufferFreeBytes
 * brief:                           get free size of ring rx buffer
 * param {ringbuf_t} *rb            pointer to ringbuffer
 * return {*}                       -1: ring buff had been overwrited; >0: free size number
 *************************************************************************************/
int ringbuffer_RxBufferFreeBytes(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_TxBufferBytesUsed
 * brief:                           get bytes used in ring tx buffer
 * param {*}                        pointer to ringbuffer
 * return {*}                       used buffer size
 *************************************************************************************/
unsigned int ringbuffer_TxBufferBytesUsed(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_RxBufferBytesUsed
 * brief:                           get bytes used in ring rx buffer
 * param {*}                        pointer to ringbuffer
 * return {*}                       used buffer size
 *************************************************************************************/
unsigned int ringbuffer_RxBufferBytesUsed(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_IsTxBufferFull
 * brief:                           check if ring tx buffer full or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: full; -1: not full
 *************************************************************************************/
int ringbuffer_IsTxBufferFull(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuffer_IsRxBufferFull
 * brief:                           check if ring rx buffer full or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: full; -1: not full
 *************************************************************************************/
int ringbuffer_IsRxBufferFull(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuf_IsTxBufferEmpty
 * brief:                           check if ring tx buffer empty or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: empty; -1: not empty
 *************************************************************************************/
int ringbuf_IsTxBufferEmpty(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                            ringbuf_IsRxBufferEmpty
 * brief:                           check if ring rx buffer empty or not
 * param {*}                        pointer to ring buffer
 * return {*}                       0: empty; -1: not empty
 *************************************************************************************/
int ringbuf_IsRxBufferEmpty(const ringbuffer_t *pRb);

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferWriteIn
 * brief:                                   write in data into rx ring buffer
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *dat               pointer to data which will be wrote into ring buffer
 * return {*}                               0: success to write in; -1: ring buffer pointer not exist/error
 *************************************************************************************/
int ringbuffer_RxBufferWriteIn(ringbuffer_t *pRb, unsigned char *dat);

/*************************************************************************************
 * func:                                    ringbuffer_FindCharInRxBuffer
 * brief:                                   find specified character in ring rx buffer
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *dat               specified char
 * return {*}                               -1: not found;  >=0: locationIndex
 *************************************************************************************/
int ringbuffer_FindCharInRxBuffer(ringbuffer_t *pRb, unsigned char *dat);

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferReadOut
 * brief:                                   read out size data into pBuf
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * param {unsigned char} *pBuf              pointer to data which will be wrote into ring buffer
 * param {unsigned int} size                how many bytes been read out
 * return {*}                               0: success to write in; -1: ring buffer pointer not exist/error
 *************************************************************************************/
int ringbuffer_RxBufferReadOut(ringbuffer_t *pRb, unsigned char *pBuf, unsigned int size);

/*************************************************************************************
 * func:                                    ringbuffer_RxBufferMemberPrint
 * brief:                                   printf rx buffer member
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * return {*}                               None
 *************************************************************************************/
void ringbuffer_RxBufferMemberPrint(ringbuffer_t *pRb);

/*************************************************************************************
 * func:                                    ringbuffer_TxBufferMemberPrint
 * brief:                                   printf tx buffer member
 * param {ringbuffer_t} *pRb                pointer to ring buffer
 * return {*}                               None
 *************************************************************************************/
void ringbuffer_TxBufferMemberPrint(ringbuffer_t *pRb);


#endif /* __RINGBUFFER_H___ */
