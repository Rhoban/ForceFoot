/*****************************************************************************/
/* Byte queue                                                                */
/* Author : Olivier Ly                                                       */
/* Date : 2008/07                                                            */
/*****************************************************************************/
#include <dxl/types.h>
#include <dxl/byte_queue.h>

inline
int incr_queue_index(ui8_queue * queue, int i) {
  register int idx = i;
  idx++;
  if (idx == queue->q_size)
    idx=0;
  return idx;
}

void init_queue(ui8_queue * queue, ui8 * buffer, int size) {
  queue->q_buffer = buffer;
  queue->q_size = size;
  reset_queue(queue);
}

void reset_queue(ui8_queue * queue) {
  queue->q_start=0;
  queue->q_end=0;
}

int queue_byte_nb(ui8_queue * queue) {
  if (queue->q_end >= queue->q_start)
    return queue->q_end - queue->q_start;
  else
    return queue->q_end + (queue->q_size - queue->q_start);
}

ui8 queue_get_byte(ui8_queue * queue) {
  if (queue->q_start == queue->q_end)
    return 0;
  
  ui8 c = queue->q_buffer[queue->q_start];
  queue->q_start = incr_queue_index(queue, queue->q_start);
  return c;
}

ui8 queue_get_nth_byte(ui8_queue * queue, int n) {
  if (n >= queue_byte_nb(queue)) return 0; // ERROR

  register int idx = queue->q_start + n;
  if (idx > queue->q_size)
    idx = idx - queue->q_size;
  return queue->q_buffer[idx];
}

/* TODO : error if the queue is full */
void queue_push_byte(ui8_queue * queue, ui8 b) {
  int idx = incr_queue_index(queue, queue->q_end);
  
  if (idx == queue->q_start) {   /* if the queue is full, error */
    // TODO: queue overfull error
    return;
  }
  
  queue->q_buffer[queue->q_end] = b;
  queue->q_end = idx;
}


/*****************************************************************************/
/*****************************************************************************/
