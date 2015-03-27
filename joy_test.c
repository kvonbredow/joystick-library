#include "joystick.h"

void print_event( struct js_event e ) {
  printf("Button %d was %s.\n", e.number, e.value ? "pressed" : "released");
  //printf("Button %d has value %d.\n", e.number, e.value);
}

int main() {
  init_listener(JOY_0);

  register_listener( &print_event, BUTTONS_ONLY );
  //register_listener( &print_event, AXIS_ONLY );

sleep(3600);
  
  return 0;
}
