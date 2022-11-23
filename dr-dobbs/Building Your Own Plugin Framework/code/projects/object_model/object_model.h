#ifndef OBJECT_MODEL
#define OBJECT_MODEL

#include "c_object_model.h"

typedef C_ActorInfo ActorInfo;

struct IActorInfoIterator
{
  virtual void reset() = 0;
  virtual ActorInfo * next() = 0;
};

struct ITurn
{
  virtual ActorInfo * getSelfInfo() = 0;
  virtual IActorInfoIterator * getFriends() = 0;
  virtual IActorInfoIterator * getFoes() = 0;

  virtual void move(apr_uint32_t x, apr_uint32_t y) = 0;
  virtual void attack(apr_uint32_t id) = 0;
};

struct IActor
{
  virtual void getInitialInfo(ActorInfo * info) = 0;
  virtual void play(ITurn * turnInfo) = 0;
};

#endif
