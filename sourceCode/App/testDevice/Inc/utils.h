/*
 * utils.h
 *
 *  Created on: Sep 5, 2023
 *      Author: icepe
 */
#include <stdint.h>

typedef struct {
    uint8_t * const buffer;
    uint16_t head;
    uint16_t tail;
    const uint16_t maxlen;
    uint16_t size;
} circ_bbuf_t;


#define CIRC_BBUF_DEF(x,y)                \
    uint8_t x##_data_space[y+1];            \
    circ_bbuf_t x = {                     \
        .buffer = x##_data_space,         \
        .head = 0,                        \
        .tail = 0,                        \
        .maxlen = y,                      \
        .size = 0                         \
    }

int16_t circ_bbuf_push(circ_bbuf_t *c, uint8_t data);
int16_t circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data);
