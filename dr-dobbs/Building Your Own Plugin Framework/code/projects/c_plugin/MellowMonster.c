

#include <object_model/c_object_model.h>
#include <stdlib.h>
#include "MellowMonster.h"

typedef struct MellowMonster_
{
  C_Actor actor;

  /* additional monster-specific data */
  apr_uint32_t dummy;

} MellowMonster;

void MellowMonster_getInitialInfo(C_ActorHandle handle, C_ActorInfo * info)
{
  MellowMonster * mm = (MellowMonster *)handle;
  strcpy((char *)info->name, "MellowMonster");
  info->attack = 10;
  info->damage = 3;
  info->defense = 8;
  info->health = 20;
  info->movement = 2;

  // Irrelevant. Will be assigned by system later
  info->id = 0;
  info->location_x = 0;
  info->location_y = 0;
}

void MellowMonster_play(C_ActorHandle handle, C_Turn * turn)
{
  MellowMonster * mm = (MellowMonster *)handle;
  C_ActorInfoIterator * friends = turn->getFriends(turn->handle);
}

void * MellowMonster_create(PF_ObjectParams * params)
{
  MellowMonster * mm = (MellowMonster *)malloc(sizeof(MellowMonster));
  mm->actor.handle = (C_ActorHandle)mm;
  mm->actor.getInitialInfo = MellowMonster_getInitialInfo;
  mm->actor.play = MellowMonster_play;

  return mm;
}

apr_int32_t MellowMonster_destroy(void * p)
{
  if (!p)
    return -1;
  free((MellowMonster *)p);
  return 0;
}

MellowMonster_init()
{
}






