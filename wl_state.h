/*
  \filename wl_state.h

  
*/
#ifndef __WL_STATE_H__
#define __WL_STATE_H__
/*
  Include section
*/
#include "id_heads.h"
#include "wl_def.h"
#include "id_us.h"




/*
   Global variables
*/

dirtype opposite[9] =
        {west,southwest,south,southeast,east,northeast,north,northwest,nodir};

dirtype diagonal[9][9] =
{
/* east */      {nodir,nodir,northeast,nodir,nodir,nodir,southeast,nodir,nodir},
                        {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
/* north */ {northeast,nodir,nodir,nodir,northwest,nodir,nodir,nodir,nodir},
                        {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
/* west */  {nodir,nodir,northwest,nodir,nodir,nodir,southwest,nodir,nodir},
                        {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
/* south */ {southeast,nodir,nodir,nodir,southwest,nodir,nodir,nodir,nodir},
                        {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
                        {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir}
};

/*
   Functions prototypes
*/
//Occupying the structure functions objtype.

void    SpawnNewObj (unsigned tilex, unsigned tiley, statetype *state);
void    NewState (objtype *ob, statetype *state);

boolean TryWalk (objtype *ob);
void    MoveObj (objtype *ob, long move);

void    KillActor (objtype *ob);
void    DamageActor (objtype *ob, unsigned damage);

boolean CheckLine (objtype *ob);
void FirstSighting (objtype *ob);
boolean CheckSight (objtype *ob);






#endif
