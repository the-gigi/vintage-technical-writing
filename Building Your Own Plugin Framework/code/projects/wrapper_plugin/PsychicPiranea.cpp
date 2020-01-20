#include "PsychicPiranea.h"

PsychicPiranea::PsychicPiranea(PF_ObjectParams *)
{
}

void PsychicPiranea::getInitialInfo(ActorInfo * info)
{
  ::strcpy((char *)info->name, "PsychicPiranea");
  info->attack = 22;
  info->damage = 2;
  info->defense = 5;
  info->health = 11;
  info->movement = 2;

  // Irrelevant. Will be assigned by system later
  info->id = 0;
  info->location_x = 0;
  info->location_y = 0;
}

void PsychicPiranea::play( ITurn * turnInfo)
{
  IActorInfoIterator * friends = turnInfo->getFriends();
}


