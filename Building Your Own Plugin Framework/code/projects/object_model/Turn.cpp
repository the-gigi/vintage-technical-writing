#include "Turn.h"
#include <iostream>
#include <algorithm>


static C_ActorInfo * getSelfInfo_(C_TurnHandle handle)
{
  ITurn * t = reinterpret_cast<Turn *>(handle);
  return t->getSelfInfo();
}

C_ActorInfoIterator * getFriends_(C_TurnHandle handle)
{
  ITurn * t = reinterpret_cast<Turn *>(handle);
  return static_cast<ActorInfoContainer *>(t->getFriends());
}

C_ActorInfoIterator * getFoes_(C_TurnHandle handle)
{
  ITurn * t = reinterpret_cast<Turn *>(handle);
  return static_cast<ActorInfoContainer *>(t->getFoes());
}

void move_(C_TurnHandle handle, apr_uint32_t x, apr_uint32_t y)
{
  ITurn * t = reinterpret_cast<Turn *>(handle);
  t->move(x, y);
}

void attack_(C_TurnHandle handle, apr_uint32_t id)
{
  ITurn * t = reinterpret_cast<Turn *>(handle);
  t->attack(id);
}

Turn::Turn()
{
  C_Turn::handle = (C_TurnHandle)this;
  C_Turn::attack = attack_;
  C_Turn::move = move_;
  C_Turn::getSelfInfo = getSelfInfo_;
  C_Turn::getFoes = getFoes_;
  C_Turn::getFriends = getFriends_;
}

Turn::~Turn()
{
}

ActorInfo * Turn::getSelfInfo()
{
  return self;
}
  
IActorInfoIterator * Turn::getFriends()
{
  return &friends;
}
  
IActorInfoIterator * Turn::getFoes()
{
  return &foes;
}

void Turn::move(apr_uint32_t x, apr_uint32_t y)
{
  self->location_x += x;
  self->location_y += y;
}

static void doSingleFightSequence(ActorInfo & attacker, ActorInfo & defender)
{
  // Check if attacker hits or misses
  bool hit = (::rand() % attacker.attack - ::rand() % defender.defense) > 0;
  if (!hit) // miss
  {
    std::cout << attacker.name << " misses " << defender.name << std::endl;
    return;
  }

  // Deal damage
  apr_uint32_t damage = 1 + ::rand() % attacker.damage;
  defender.health -= std::min(defender.health, damage);
  std::cout << attacker.name << "(" << attacker.health << ") hits " 
            << defender.name << "(" << defender.health << "), damage: " << damage << std::endl;
}

void Turn::attack(apr_uint32_t id)
{
  ActorInfo * foe = NULL;
  foes.reset();
  while ((foe = foes.next()))
    if (foe->id == id)
      break;

  // Attack only foes
  if (!foe)
    return; 

  std::cout << self->name << "(" << self->health << ") attacks " 
            << foe->name << "(" << foe->health << ")" << std::endl;
  while (true)
  {
    // first attacker attacks
    doSingleFightSequence(*self, *foe);
    if (foe->health == 0)
    {
      std::cout << self->name << " defeated " << foe->name << std::endl;
      return;
    }

    // then foe retaliates
    doSingleFightSequence(*foe, *self);
    if (self->health == 0)
    {
      std::cout << self->name << " was defeated by " << foe->name << std::endl;
      return;
    }
  }
}

