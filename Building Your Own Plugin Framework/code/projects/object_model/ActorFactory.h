
#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include "plugin_framework/PluginManager.h"
#include "plugin_framework/ObjectAdapter.h"
#include "object_model/ActorAdapter.h"

struct ActorFactory : public ObjectAdapter<ActorAdapter, C_Actor>
{
  static ActorFactory & getInstance()
  {
    static ActorFactory instance;

    return instance;
  }

  static IActor * createActor(const std::string & objectType)
  {
    void * actor = PluginManager::getInstance().createObject(objectType, getInstance());
    return (IActor *)actor;
  }
};


#endif // ACTOR_FACTORY_H
