#include "id_us.h"

/*
 Return a random number between 0-255

 NOTE: Former ASM code, 
 this version is C based.

 Initiate random generator is needed.

*/
int US_RndT(void)
{
  return rand()%256;
}

void init_random_generator( void )
{
  static int initialized = 0;

  if( initialized == 0)
  {
    srand ( time (NULL) );

    initialized == 1;
  }
}
