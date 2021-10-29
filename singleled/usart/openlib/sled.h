/**
 * @file sled.h
 * @author simon.y (Tresordie@163.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __SLED_H__
#define __SLED_H__


#define container_of(ptr, type, member) ({                      \
            const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
            (type *)( (char *)__mptr - offsetof(type,member) );})


// Single color led name max length
#define SLED_NAME_MAX       16

typedef enum {
    LED_MODENONE,
    LED_STEADYON,
    LED_STEADYOFF,
    LED_FLASH
}SLED_MODE_e;

typedef enum {
    S_RED,
    S_GREEN,
    S_BLUE,
    S_YELLOW,
    S_WHITE
}SLED_COLOR_e;

// sled class data
typedef struct {
    unsigned char   name[SLED_NAME_MAX];
    unsigned int    led_id;
    SLED_COLOR_e    sled_color;
    SLED_MODE_e     sled_mode;
    unsigned int    timegap_ticks;
}sled_t;

// sled class action
typedef void (*event_cb)(sled_t *sled);

typedef struct {
    SLED_MODE_e sled_mode;
    event_cb    sled_cb;
}sledactioncb_t;

typedef struct lednode{
    sled_t sled;
    struct lednode *lednode_prev;
    struct lednode *lednode_next;
}LedNode_t;

typedef struct ledlist{
    LedNode_t *LedHead;
    LedNode_t *LedTail;
}Ledlist_t;

void sledlist_init(void);
int sledlist_AddNode(Ledlist_t *ledlist, LedNode_t *lednode);
int sledlist_DeleteNode(Ledlist_t *ledlist, LedNode_t *lednode);
int sled_create(LedNode_t *lednode, sled_t *psParams);
void sled_attach(LedNode_t *lednode, SLED_MODE_e sled_mode, unsigned int timegap_ticks);
void sled_cycle_process(void);

#endif

