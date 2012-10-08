/*
 *author:sunzq
*/
#include <stdlib.h>
#include <unistd.h>
//#include "app.h"

#include <app/common/fifoarray.h>

int fifo_array_create(fifoArray *queue, unsigned int queue_size, unsigned int datalen)
{
  if(NULL == queue){
    return 0;
  }
  // init queue
  queue->rows = queue_size;
  queue->columns = datalen;
  queue->used_size = 0;
  queue->fetch_pos = 0;
  queue->save_pos = 0;
  pthread_mutex_init(&(queue->lock), NULL);
  int i;
  //new a points array
  queue->buff = (void **)malloc(sizeof(void *) * (queue->rows+1));
  //new a real data storage array
  queue->buff[0] = (void *)malloc(sizeof(char) * queue->rows * queue->columns);
  if (queue->buff != NULL){
    //make the storage array divide in to queue_size memory segment
    for(i = 1; i <= queue->rows; i++)
      queue->buff[i] = queue->buff[i-1] + datalen;
    return 1;
  }
  //end init
  return 0;
}


int fifo_array_put_squeeze_out(fifoArray *queue, void *data)
{
  if(NULL == queue || NULL == data){
    return 0;
  }
  if(queue->used_size == queue->rows){
    fifo_array_pop(queue);
    return fifo_array_put(queue, data);
  }else{
    return fifo_array_put(queue, data);
  }
}


int fifo_array_put(fifoArray *queue, void *data)
{
  if (NULL == queue || NULL == data || queue->used_size == queue->rows){
    return 0;
  }
  //compute the inserted index.
  //notice:treat the points array as a circular array, so we use below manner to make sure that 
  //insert an element at the last position of the queue. 
  int index = (queue->fetch_pos + queue->used_size) > (queue->rows - 1) ? 
              (queue->fetch_pos + queue->used_size)%queue->rows : 
              (queue->fetch_pos + queue->used_size);
  //save the data into real storage array.
  memcpy(queue->buff[index], data, queue->columns);
  index++;
  if (index > (queue->rows - 1)) {
      index = 0;
  }
  queue->save_pos = index;
  queue->used_size++;
  //printf("\t\t\t\t\t\tafter buff_put, used_buff=%d,fetch_pos=%d,index=%d+++++++++\n", buff->used_size, buff->fetch_pos, index);
  return 1;
}

int fifo_array_bput(fifoArray *queue, void *data)
{
  int r = 0;
  while(!(r = fifo_array_put(queue, data))){usleep(50);}
  return r;
}

void *fifo_array_get(fifoArray *queue, int index)
{
  if(NULL == queue || queue->used_size <= index || index < 0){
    return NULL;
  }
//  assert(queue->used_size >= index);
//  assert(index >= 0);
  void *buf = queue->buff[index];
  return buf;  
}

void *fifo_array_pop(fifoArray *queue)
{
  if(NULL == queue || queue->used_size == 0){
    return NULL;
  }
  //temp store
  queue->buff[queue->rows] = queue->buff[queue->fetch_pos];
  queue->used_size--;
  queue->fetch_pos++;
  if(queue->fetch_pos == queue->rows)
    queue->fetch_pos = 0;
//  printf("\t\tafter buff_pop, used_buff=%d,fetch_pos=%d---------\n", queue->used_size, queue->fetch_pos);
  void *buf = queue->buff[queue->rows];
  return buf;  
}

void *fifo_array_bpop(fifoArray *queue)
{
  void * r = NULL;
  while(NULL == (r = fifo_array_pop(queue))){usleep(50);}
  return r;
}

int fifo_pthread_mutex_lock(fifoArray *queue)
{
  return pthread_mutex_lock(&(queue->lock));
}

int fifo_pthread_mutex_unlock(fifoArray *queue)
{
  return pthread_mutex_unlock(&(queue->lock));
}

int fifo_pthread_mutex_trylock(fifoArray *queue)
{
  return pthread_mutex_trylock(&(queue->lock));
}

int fifo_array_destroy(fifoArray *queue)
{
  if(NULL != queue->buff){
printf("destroy fifo buff->buff[0]=%p,buff->buff=%p\n",queue->buff[0],queue->buff);
    free(queue->buff[0]);
    free(queue->buff);
    pthread_mutex_destroy(&(queue->lock));
    queue->buff = NULL;
    return 1;
  }
  return 0;
}
