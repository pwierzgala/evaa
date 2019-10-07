#include "fsm.h"
#include <stdio.h>

int main()
{
  printf("In main!\n");
  fsm_t *fsm = get_instance_fsm();
  init_fsm(fsm);
  fsm->run();
  printf("Good bye!\n");
  return 0;
}
