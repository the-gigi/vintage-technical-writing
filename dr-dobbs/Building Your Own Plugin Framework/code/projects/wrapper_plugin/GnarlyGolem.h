
#ifndef GNARLY_GOLEM_H
#define GNARLY_GOLEM_H

#include <object_model/object_model.h>
#include <object_model/ActorBaseTemplate.h>

struct PF_ObjectParams;

class GnarlyGolem : public ActorBaseTemplate<GnarlyGolem>
{
public:
  GnarlyGolem(PF_ObjectParams *);

  // IActor methods
  virtual void getInitialInfo(ActorInfo * info);
  virtual void play(ITurn * turnInfo);
  
private:
  GnarlyGolem();
};

#endif
