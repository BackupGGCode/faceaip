/*
 *author : sunzq
*/

#ifndef FIFOARRAY_H_
#define FIFOARRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>

#include <pjlib.h>

#include <sys/types.h>
#include "../server/types.h"
#include <app/datagram/IPCProtocolDef.h>

#include "tshome_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
init a fifoArray.
@param queue_size max capacity of the array
@param data_len  size of a element
*/
int fifo_array_create(fifoArray *queue, unsigned int queue_size, unsigned int data_len);
/*
put a element into fifoArray whatever. if the queue is full, pop one and then put this.
*/
int fifo_array_put_squeeze_out(fifoArray *queue, void *data);
/*
put a element into the queue
*/
int fifo_array_put(fifoArray *queue, void *data);
/*
A blocked manner of fifo_array_put().
*/
int fifo_array_bput(fifoArray *queue, void *data);
/*
get value with the specified index.and it remain in the queue
*/
void *fifo_array_get(fifoArray *queue, int index);
/*
pop value at index = 0.
*/
void *fifo_array_pop(fifoArray *queue);
/*
A blocked manner of fifo_array_pop().
*/
void *fifo_array_bpop(fifoArray *queue);
/*
lock the queue
*/
int fifo_pthread_mutex_lock(fifoArray *queue);
/*
unlock the queue.
*/
int fifo_pthread_mutex_unlock(fifoArray *queue);
/*
trylock.according to the current OS.
*/
int fifo_pthread_mutex_trylock(fifoArray *queue);
/*
release all memery.
*/
int fifo_array_destroy(fifoArray *queue);

#ifdef __cplusplus
}
#endif

#endif/*FIFOARRAY_H_*/
