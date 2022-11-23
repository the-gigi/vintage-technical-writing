#ifndef HERO_H
#define HERO_H

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "object_model/object_model.h"

class Hero : public IActor
{
public:
  Hero();
  ~Hero();

  // IActor methods
  virtual void getInitialInfo(ActorInfo * info);
  virtual void play(ITurn * turnInfo);
  
private:

};

#endif
