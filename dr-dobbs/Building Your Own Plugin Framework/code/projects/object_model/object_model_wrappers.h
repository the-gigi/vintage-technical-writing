#ifndef OBJECT_MODEL_WRAPPERS_H
#define OBJECT_MODEL_WRAPPERS_H

#include <string>
#include <vector>
#include <map>

#include "object_model.h"
#include "c_object_model.h"

struct ActorInfoIteratorWrapper : public IActorInfoIterator
{
  ActorInfoIteratorWrapper(C_ActorInfoIterator * iter) : index_(0)
  {
    iter->reset(iter->handle);
    
    // Create an internal vector of ActorInfo objects
    const ActorInfo * ai = NULL;
    while ((ai = iter->next(iter->handle)))
      vec_.push_back(*ai);
  }
  
  // IActorInfoIteraotr methods
  virtual void reset()
  {
    index_ = 0;
  }

  virtual ActorInfo * next()
  {
    if (index_ == vec_.size())
      return NULL;
      
    return &vec_[index_++];
  }
  
private:
  apr_uint32_t index_;
  std::vector<ActorInfo> vec_;
};

struct TurnWrapper : public ITurn
{
  TurnWrapper(C_Turn * turn) : 
    turn_(turn),
    friends_(turn->getFriends(turn->handle)),
    foes_(turn->getFoes(turn->handle))
  { 
  }
  
  // ITurn methods
  virtual ActorInfo * getSelfInfo()
  {
    return turn_->getSelfInfo(turn_->handle);
  }

  virtual IActorInfoIterator * getFriends()
  {
    return &friends_;
  }
  virtual IActorInfoIterator * getFoes()
  {
    return &foes_;
  }

  virtual void move(apr_uint32_t x, apr_uint32_t y)
  {
    turn_->move(turn_->handle, x, y);
  }

  virtual void attack(apr_uint32_t id)
  {
    turn_->attack(turn_->handle, id);
  }

private:
  C_Turn * turn_;
  ActorInfoIteratorWrapper friends_;
  ActorInfoIteratorWrapper foes_;
};
  
#endif // OBJECT_MODEL_WRAPPERS_H



