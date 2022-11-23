
#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>
#include <plugin_framework/PluginManager.h>
#include "Hero.h"

class BattleManager
{
  static const apr_int32_t MONSTER_COUNT = 10;
  static const apr_int32_t BATTLEFIELD_WIDTH = 10;
  static const apr_int32_t BATTLEFIELD_HEIGHT = 10;

  typedef std::pair<apr_int32_t, apr_int32_t> Location;
  typedef std::vector<std::string> MonsterTypeVec; 
  typedef std::map<IActor *, ActorInfo> ActorMap;
  typedef std::list<ActorInfo *> Faction;

public:
  static BattleManager & getInstance();
  void go();
  
private:
  BattleManager();
  ~BattleManager();

  IActor * createRandomMonster(const PluginManager::RegistrationMap & rm);
  void playTurn();

private:
  bool gameOver_;
  Hero hero_;
  Faction heroFaction_;
  Faction enemyFaction_;
  std::map<Location, ActorInfo *> battlefield_;

 ActorMap actors_;
  MonsterTypeVec monsterTypes_;
};

#endif
