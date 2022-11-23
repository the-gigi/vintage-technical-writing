#include "GnarlyGolem.h"

GnarlyGolem::GnarlyGolem(PF_ObjectParams *)
{
}

void GnarlyGolem::getInitialInfo(ActorInfo * info)
{
  ::strcpy((char *)info->name, "GnarlyGolem");
  info->attack = 1;
  info->damage = 500;
  info->defense = 10;
  info->health = 200;
  info->movement = 1;

  // Irrelevant. Will be assigned by system later
  info->id = 0;
  info->location_x = 0;
  info->location_y = 0;
}

void GnarlyGolem::play( ITurn * turnInfo)
{
  const IActorInfoIterator * friends = turnInfo->getFriends();
}

