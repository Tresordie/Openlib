/**
 * @file sled.c
 * @author simon.y (Tresordie@163.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "sled.h"
#include "bsp.h"

static Ledlist_t ledlist;

// define static variety timegap for led flash gap
static unsigned int led1flash_timegap = 0;
static unsigned int led2flash_timegap = 0;

void sled_steadyoff(sled_t *sled);
void sled_steadyon(sled_t *sled);
void sled_flash(sled_t *sled);

sledactioncb_t sled_act[3] = {
    {LED_STEADYOFF, sled_steadyoff},
    {LED_STEADYON,  sled_steadyon},
    {LED_FLASH,     sled_flash},
};

/**
 * @brief   Initialize single color indicator led list
 * 
 * @param ledlist 
 */
void sledlist_init(void)
{
    ledlist.LedHead = ledlist.LedTail = NULL;
}

/**
 * @brief Add sled node to led list end
 *        for double direction list, list head == list tail
 * 
 * @param ledlist 
 * @param lednode 
 * @return int  -1:error, 0:finished
 */
int sledlist_AddNode(Ledlist_t *ledlist, LedNode_t *lednode) 
{
    // ledlist and lednode not equal to NULL
    if ((NULL == ledlist) || (NULL == lednode))
        return (-1);

    // check if the node to add is the first node(as Head)
    if ((NULL == ledlist->LedHead) && (ledlist->LedHead == ledlist->LedTail)) {
        ledlist->LedHead = lednode;
        ledlist->LedTail = lednode;
    }else {
        lednode->lednode_prev = ledlist->LedTail;
        lednode->lednode_next = ledlist->LedHead;

        ledlist->LedTail->lednode_next = lednode;
        ledlist->LedHead->lednode_prev = lednode;

        ledlist->LedTail               = lednode;
    }

    return 0;
}

/**
 * @brief Delete node from led list
 * 
 * @param ledlist specified led list
 * @param lednode specified led node
 * @return int    -1: error; 0: 
 */
int sledlist_DeleteNode(Ledlist_t *ledlist, LedNode_t *lednode)
{
    if ((NULL == ledlist) || (NULL == lednode))
        return (-1);

    if (lednode == ledlist->LedHead) {
        lednode->lednode_next->lednode_prev = ledlist->LedTail;
        lednode->lednode_prev->lednode_next = lednode->lednode_next;

        ledlist->LedHead = ledlist->LedHead->lednode_next;
    }else if (lednode == ledlist->LedTail) {
        lednode->lednode_next->lednode_prev = lednode->lednode_prev;
        lednode->lednode_prev->lednode_next = lednode->lednode_next;

        ledlist->LedTail = lednode->lednode_prev;
    }else {
        lednode->lednode_next->lednode_prev = lednode->lednode_prev;
        lednode->lednode_prev->lednode_next = lednode->lednode_next;
    }
    return 0;
}

/**
 * @brief           create a sled node
 * 
 * @param lednode   receive pointer to sled node created
 * @param psParams  defined sled data pointer
 * @return int      -1: error, 0: correct
 */
int sled_create(LedNode_t *lednode, sled_t *psParams)
{
    if (NULL == psParams)
        return (-1);

    lednode = (LedNode_t *)malloc(sizeof(LedNode_t));

    if (NULL == lednode)
        return (-1);

    memset(&lednode->sled, 0, sizeof(sled_t));
    memcpy(lednode->sled.name, psParams->name, SLED_NAME_MAX);

    // when led node created, can specify sled_act callback function or specify by some special event
    lednode->sled.led_id        = psParams->led_id;
    lednode->sled.sled_color    = psParams->sled_color;
    lednode->sled.sled_mode     = psParams->sled_mode;
    lednode->sled.timegap_ticks = psParams->timegap_ticks;

    sledlist_AddNode(&ledlist, lednode);

    return 0;
}

/**
 * @brief   connect lednode to specified mode, callback function
 * 
 * @param lednode           sled node
 * @param sled_mode         sled mode
 * @param timegap_ticks     only make sense when in flash mode
 * @param sled_act          callback function pointer
 */
void sled_attach(LedNode_t *lednode, SLED_MODE_e sled_mode, unsigned int timegap_ticks)
{
    lednode->sled.sled_mode     = sled_mode;
    lednode->sled.timegap_ticks = timegap_ticks;
}

/**
 * @brief single color led keep OFF state
 * 
 * @param sled  sled class data type as params
 */
void sled_steadyoff(sled_t *sled)
{
    switch (sled->led_id) {
        case 1:
            LED1_OFF;
            break;

        case 2:
            LED2_OFF;
            break;

        default:
            break;
    }
}

/**
 * @brief single color led keep ON state
 * 
 * @param sled  sled class data type as params
 */
void sled_steadyon(sled_t *sled)
{
    switch (sled->led_id) {
        case 1:
            LED1_ON;
            break;

        case 2:
            LED2_ON;
            break;

        default:
            break;
    }
}

/**
 * @brief single color led keep flash state
 * 
 * @param sled  sled class data type as params
 */
void sled_flash(sled_t *sled)
{
    switch (sled->led_id) {
        case 1:
            if (!led1flash_timegap) {
                LED1_TOGGLE;
                led1flash_timegap = sled->timegap_ticks;
            }
            break;

        case 2:
            if (!led2flash_timegap) {
                LED2_TOGGLE;
                led2flash_timegap = sled->timegap_ticks;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief polling all sled node in sled list, if sled_mode specified, then callback function and run
 * 
 */
void sled_cycle_process(void)
{
    unsigned char i = 0;
    unsigned char cbsize = 0;
    LedNode_t *psLedNode = NULL;

    cbsize = (sizeof(sled_act))/(sizeof(sled_act[0]));

    for (psLedNode = ledlist.LedHead; psLedNode != NULL; psLedNode = psLedNode->lednode_next) {
        for (i = 0; i < cbsize; i++) {
            if (sled_act[i].sled_mode == psLedNode->sled.sled_mode) {
                sled_act[i].sled_cb(&psLedNode->sled);
            }
        }
    }
}

/**
 * @brief   Systick_Handler used as timer of led flash time gap
 *          Systick_Handler also can be defined in other source file
 * 
 */
void SysTick_Handler(void)
{
    if (led1flash_timegap)
        led1flash_timegap--;

    if (led2flash_timegap)
        led2flash_timegap--;
}


