#include "Hero.h"
#include <plugin_framework/plugin.h>
#include <vector>
#include <iostream>
#include <utils/utils.h>

Hero::Hero()
{
}

Hero::~Hero()
{
}

void Hero::getInitialInfo(ActorInfo * info)
{
  ::strcpy((char *)info->name, "Hero");
  info->attack = 8;
  info->damage = 5;
  info->defense = 8;
  info->health = 350;
  info->movement = 1;

  // Irrelevant. Will be assigned by system later
  info->id = 0;
  info->location_x = 0;
  info->location_y = 0;
}

void Hero::play(ITurn * turnInfo)
{
  // Find enemy with the poorest health
  const ActorInfo * weakest = NULL;
  const ActorInfo * f = NULL;;
  while (f = turnInfo->getFoes()->next())
  {
    if (!weakest || f->health < weakest->health)
      weakest = f;
  }

  if (!weakest) // no enemies, nothing to do
    return;

  std::cout << "Hero::play(), "
    << "Weakest enemy - name: " << weakest->name
    << " health: " << weakest->health << std::endl;

  
  // Chase the weakest and attack when you catch it
  const ActorInfo * self = turnInfo->getSelfInfo();
  
  Position p1(self->location_x, self->location_y);
  Position p2(weakest->location_x, weakest->location_y);

  Position closest = findClosest(p1, p2, self->movement);
  turnInfo->move(closest.first, closest.second);
  if (closest == p2)
    turnInfo->attack(weakest->id);
}

