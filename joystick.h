#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

#define DEFAULT 0x00
#define AXIS_ONLY 0x01
#define BUTTONS_ONLY 0x02
#define NON_BLOCKING 0x04

#define NUM_L 16

#define JOY_0 "/dev/input/js0"
#define JOY_1 "/dev/input/js1"
#define JOY_2 "/dev/input/js2"
#define JOY_3 "/dev/input/js3"
#define JOY_4 "/dev/input/js4"

struct js_event {
  unsigned int time;      /* event timestamp in milliseconds */
  short value;            /* value */
  unsigned char type;     /* event type */
  unsigned char number;   /* axis/button number */
};

struct listener_handle {
  void (*func_ptr)(struct js_event e);
  char flags;
  char active;
};

struct nb_info {
  void (*func_ptr)(struct js_event e);
  struct js_event e;
};

struct joystick_t {
  char initialized;
  struct listener_handle registrar[NUM_L];
  int fp;
  int num_active;
  pthread_t dispatcher;
};

extern void init_listener( struct joystick_t *j, const char *filename);
extern int register_listener( struct joystick_t *j, void (*func_ptr)(struct js_event e), char flags );
void *dispatch( void * ptr );
extern void deregister_listener( struct joystick_t *j, int k );

#endif
