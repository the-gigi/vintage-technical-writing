#ifndef ACTOR_INFO_CONTAINER_H
#define ACTOR_INFO_CONTAINER_H

#include "object_model.h"
#include <vector>

struct ActorInfoContainer : 
  IActorInfoIterator,
  C_ActorInfoIterator
{
  static void reset_(C_ActorInfoIteratorHandle handle)
  {
    ActorInfoContainer * aic = reinterpret_cast<ActorInfoContainer *>(handle);
    aic->reset();
  }

  static C_ActorInfo * next_(C_ActorInfoIteratorHandle handle)
  {
    ActorInfoContainer * aic = reinterpret_cast<ActorInfoContainer *>(handle);
    return aic->next();
  }

  ActorInfoContainer() : index(0)
  {
    C_ActorInfoIterator::handle = (C_ActorInfoIteratorHandle)this;
    C_ActorInfoIterator::reset = reset_;
    C_ActorInfoIterator::next = next_;
  }

  void reset()
  {
    index = 0;
  }
  
  ActorInfo * next()
  {
    if (index >= vec.size())
      return NULL;

    return vec[index++];
  }

  apr_uint32_t index;
  std::vector<ActorInfo *> vec; 
};

#endif
