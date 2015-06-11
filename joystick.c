#include "joystick.h"

extern void init_listener( struct joystick_t *j, const char *filename ) {
  int k;
  for ( k = 0; k < NUM_L; k++ ) {
    j->registrar[k].func_ptr = NULL;
    j->registrar[k].flags = DEFAULT;
    j->registrar[k].active = 0;
  }
  j->num_active = 0;
  
  j->fp = open ( filename, O_RDONLY); 
  if (j->fp < 0) {
    printf("Unable to open %s\n", filename);
    exit(1);
  }

  j->initialized = 1;
}

extern int register_listener( struct joystick_t *j, void (*func_ptr)(struct js_event e), char flags ) {

  if ( !j->initialized ) {
    printf("Listener was not initialized");
    exit(1);
  }

  int k;
  for ( k = 0; k < NUM_L && j->registrar[k].active; k++);

  struct listener_handle *h = &j->registrar[k];
  h->func_ptr = func_ptr;
  h->flags = flags;
  h->active = 1;

  if ( !j->num_active )
    pthread_create( &j->dispatcher, NULL, dispatch, (void *)j );

  j->num_active++;

  return k;

}

//IMPLEMENT NON_BLOCKING
void *dispatch( void * ptr ) {

  struct joystick_t *j = (struct joystick_t*) ptr;
  struct js_event e;
  int bytes;
  int k;

  do {

    bytes = read( j->fp, &e, sizeof(e) );
    if ( bytes != sizeof(e) ) {
      printf("Unexpected size.\n");
    }

    e.type &= ~JS_EVENT_INIT;
 
    for ( k = 0; k < NUM_L; k++ ) {
      if ( !j->registrar[k].active ) continue;
      if ( e.type & j->registrar[k].flags ) continue;
      (*j->registrar[k].func_ptr)(e);      
    }
    
    usleep(1000);
  
  } while ( 1 );

}

extern void deregister_listener( struct joystick_t *j, int k ) {

  struct listener_handle *h = &j->registrar[k];
  if ( !h->active ) return;
  h->active = 0;
  h->func_ptr = NULL;
  h->flags = DEFAULT;

  j->num_active--;

  if ( !j->num_active ) {
    pthread_cancel(j->dispatcher);
  }

}

void noblock_function( void * ptr ) {
  struct nb_info *info = (struct nb_info*) ptr;
  (*info->func_ptr)(info->e);
  free( info );
}
