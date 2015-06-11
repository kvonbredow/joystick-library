#include "joystick.h"

void print_event( struct js_event e ) {
  printf("Button %d was %s.\n", e.number, e.value ? "pressed" : "released");
}

int main( int argc, char **argv ) {

  struct joystick_t j;

  const char *js = argc > 1 ? argv[1] : JOY_0;

  init_listener( &j, js );

  register_listener( &j, &print_event, BUTTONS_ONLY );

  sleep(3600);
  
  return 0;
}
