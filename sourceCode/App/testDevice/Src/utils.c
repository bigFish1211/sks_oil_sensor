/*
 * utils.c
 *
 *  Created on: Sep 5, 2023
 *      Author: icepe
 */


#include "utils.h"

int16_t circ_bbuf_push(circ_bbuf_t *c, uint8_t data)
{
    if (c->head == c->tail)  // if the head == tail, we don't have any data
    {
        c->tail = c->head = 0;
        c->size = 0;
    }
    if (c->tail == c->maxlen -1)
    {
        c->buffer[c->tail] = data;
        c->tail = 0;
        if(c->head==0)
        {
            c->size =  0;
            c->tail  = 0;
            return -1;
        }
        c->size++;
        return 0;
    }

    c->size++;
    c->buffer[c->tail] = data;  // Load data and then move
    c->tail++;             // head to next data offset.
    if (c->size > c->maxlen)
    {
        c->tail = c->head = 0;
        c->size = 0;
        return -1;
    }
    return 0;  // return success to indicate successful push.
}

int16_t circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data)
{
    if (c->head == c->tail)  // if the head == tail, we don't have any data
    {
        c->tail = c->head = 0;
        c->size = 0;
        return -1;
    }

    if (c->head == c->maxlen-1)
    {
        *data = c->buffer[c->head];
        c->head = 0;
        c->size--;
        return 0;
    }

    c->size--;
    *data = c->buffer[c->head];  // Read data and then move
    c->head++;           // tail to next offset.
    return 0;  // return success to indicate successful push.
}
