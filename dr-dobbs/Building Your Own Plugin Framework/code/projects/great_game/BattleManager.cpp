#include "BattleManager.h"
#include <object_model/Turn.h>
#include <object_model/ActorFactory.h>
#include <plugin_framework/PluginManager.h>
#include <plugin_framework/plugin.h>

#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>

BattleManager & BattleManager::getInstance()
{
  static BattleManager bm;

  return bm;
}

BattleManager::BattleManager()
{

}

BattleManager::~BattleManager()
{
}

void BattleManager::go()
{
  // Get all monster types
  PluginManager & pm = PluginManager::getInstance();
  const PluginManager::RegistrationMap & rm = pm.getRegistrationMap();

  for (PluginManager::RegistrationMap::const_iterator i = rm.begin(); i != rm.end(); ++i)
  {
    monsterTypes_.push_back(i->first);
  }

  // Dump all the monsters
  for (MonsterTypeVec::iterator i = monsterTypes_.begin(); i != monsterTypes_.end(); ++i)
  {
    std::string m = *i;
    std::cout << m.c_str() << std::endl;
  }

  // Add the Hero to its faction (later allies may join)
  ActorInfo ai, heroInfo;
  hero_.getInitialInfo(&heroInfo);
  // Don't keep the hero's IActor *, because she is treated differently
  actors_.insert(std::make_pair((IActor *)0, heroInfo));
  heroFaction_.push_back(&actors_[0]);

  // Instantiate some monsters randomly
  for (apr_int32_t i = 0; i < MONSTER_COUNT; ++i)
  {
    IActor * monster = createRandomMonster(rm);
    monster->getInitialInfo(&ai);
    ai.id = i+1; // Hero is id 0
    actors_.insert(std::make_pair(monster, ai));
    
    enemyFaction_.push_back(&actors_[monster]);
  }

  while (!gameOver_)
  {
    playTurn();
  }

  heroInfo = actors_[0];
  if (heroInfo.health > 0)
    std::cout << "Hero is victorious!!!" << std::endl;
  else
    std::cout << "Hero is dead :-(" << std::endl;
}

IActor * BattleManager::createRandomMonster(const PluginManager::RegistrationMap & rm)
{
  // Select monster type
  apr_size_t index = ::rand() % monsterTypes_.size();
  const std::string & key = monsterTypes_[index];
  const PF_RegisterParams & rp = rm.find(key)->second;
  // Create it
  IActor * monster = ActorFactory::createActor(key);
  
  return monster;
}

static bool isDead(ActorInfo * ai)
{
  return ai->health == 0;
}

void BattleManager::playTurn()
{
  // Iterate over all actors (start with Hero)
  //For each actor prepare turn info (friends and foes)
  Turn t;

  ActorInfo & ai = actors_[(IActor *)0];
  t.self = &ai;
  std::copy(heroFaction_.begin(), heroFaction_.end(), std::back_inserter(t.friends.vec));
  std::copy(enemyFaction_.begin(), enemyFaction_.end(), std::back_inserter(t.foes.vec));
  hero_.play(&t);

  ActorInfo * p = NULL;
  ActorMap::iterator it;
  for (it = actors_.begin(); it != actors_.end(); ++it)
  {
    if (!it->first || isDead(&it->second))
      continue;
    
    t.self = &(it->second);
    std::copy(heroFaction_.begin(), heroFaction_.end(), std::back_inserter(t.foes.vec));
    std::copy(enemyFaction_.begin(), enemyFaction_.end(), std::back_inserter(t.friends.vec));

    it->first->play(&t);
  }
  
  // Clean up dead enemies
  Faction::iterator last = std::remove_if(enemyFaction_.begin(), enemyFaction_.end(), isDead);
  while (last != enemyFaction_.end())
  {
    enemyFaction_.erase(last++);
  }

  // Check if game is over (hero dead or all enemies are dead)
  if (isDead(&actors_[(IActor *)0]) || enemyFaction_.empty())
  {
    gameOver_ = true;
    return;
  }
}
