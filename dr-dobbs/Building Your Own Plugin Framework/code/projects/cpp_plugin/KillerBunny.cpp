#include "KillerBunny.h"
#include <string.h>



void * KillerBunny::create(PF_ObjectParams *)
{
  return new KillerBunny();
}

apr_int32_t KillerBunny::destroy(void * p)
{
  if (!p)
    return -1;
  delete (KillerBunny *)p;
  return 0;
}

KillerBunny::KillerBunny()
{
}

KillerBunny::~KillerBunny()
{
}

void KillerBunny::getInitialInfo(ActorInfo * info)
{
  ::strcpy((char *)info->name, "KillerBunny");
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

void KillerBunny::play(ITurn * turnInfo)
{
  IActorInfoIterator * friends = turnInfo->getFriends();
}


