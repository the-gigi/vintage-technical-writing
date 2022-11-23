
#ifndef PSYCHIC_PIRANEA_H
#define PSYCHIC_PIRANEA_H

#include <object_model/object_model.h>
#include <object_model/ActorBaseTemplate.h>

struct PF_ObjectParams;

class PsychicPiranea : public ActorBaseTemplate<PsychicPiranea, IActor>
{
public:
  PsychicPiranea(PF_ObjectParams *);
  // IActor methods
  virtual void getInitialInfo(ActorInfo * info);
  virtual void play(ITurn * turnInfo);
};

#endif
