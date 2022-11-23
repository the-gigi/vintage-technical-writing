#include "StationarySatan.h"
#include <string.h>


void * StationarySatan::create(PF_ObjectParams *)
{
  return new StationarySatan();
}

apr_int32_t StationarySatan::destroy(void * p)
{
  if (!p)
    return -1;
  delete (StationarySatan *)p;
  return 0;
}

StationarySatan::StationarySatan()
{
}

StationarySatan::~StationarySatan()
{
}

void StationarySatan::getInitialInfo(ActorInfo * info)
{
  ::strcpy((char *)info->name, "StationarySatan");
  info->attack = 5;
  info->damage = 20;
  info->defense = 12;
  info->health = 120;
  info->movement = 0; // It's stationary after all

  // Irrelevant. Will be assigned by system later
  info->id = 0;
  info->location_x = 0;
  info->location_y = 0;
}

void StationarySatan::play( ITurn * turnInfo)
{
  const IActorInfoIterator * friends = turnInfo->getFriends();
}

