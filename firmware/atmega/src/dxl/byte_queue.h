/*****************************************************************************/
/* Byte queue                                                                */
/* Date : 2008/07                                                            */
/*****************************************************************************/
#ifndef QUEUE_H
#define QUEUE_H

/* General use ui8 queue.
 * The queue size must be less than 255 byte, because of the size of 
 * the indexes */

/*****************************************************************************/
/* Data Structure                                                            */
/*****************************************************************************/

/* Note that the real size of the queue is size-1. Indeed, in order to save space
   we do not use any indicator to distinguish the empty state from the full state.
   Therefore, we forbid q_end to come over q_start after a complete turn of the
   queue. */

typedef struct {
  ui8 * q_buffer;
  int q_size;
  int q_start;
  int q_end;
} ui8_queue;

/*****************************************************************************/
/* Interface                                                                 */
/*****************************************************************************/
int incr_queue_index(ui8_queue * queue, int i);

void init_queue(ui8_queue * queue, ui8 * buffer, int size);
void reset_queue(ui8_queue * queue);
int queue_byte_nb(ui8_queue * queue);

/* get a new byte from the queue.
 * Caution: if the queue is empty, return 0. */
ui8 queue_get_byte(ui8_queue * queue);

/* get the nth byte from the beginning of the queue */
ui8 queue_get_nth_byte(ui8_queue * queue, int n);
void queue_push_byte(ui8_queue * queue, ui8 b);

#endif /* QUEUE_H */
/*****************************************************************************/
/*****************************************************************************/
