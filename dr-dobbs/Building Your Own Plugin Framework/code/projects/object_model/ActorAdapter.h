#ifndef ACTOR_ADAPTER_H
#define ACTOR_ADAPTER_H

//----------------------------------------------------------------------

#include "object_model/object_model.h"

//----------------------------------------------------------------------

class ActorAdapter : public IActor
{
public:
  ActorAdapter(C_Actor * actor, PF_DestroyFunc destroyFunc) : 
    actor_(actor),
    destroyFunc_(destroyFunc)
  {
  }
  
  ~ActorAdapter()
  {
    if (destroyFunc_)
      destroyFunc_(actor_);
  } 

  // IActor implememntation
  void getInitialInfo(ActorInfo * info)
  {
    actor_->getInitialInfo(actor_->handle, info);
  }

  void play(ITurn * turnInfo)
  {
    actor_->play(actor_->handle, static_cast<Turn *>(turnInfo));
  }
  
private:
  C_Actor *  actor_;
  PF_DestroyFunc destroyFunc_;
};

#endif // ACTOR_ADAPTER_H
