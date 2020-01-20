
#ifndef STATIONARY_SATAN_H
#define STATIONARY_SATAN_H

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <object_model/object_model.h>

struct PF_ObjectParams;

class StationarySatan : public IActor
{
public:

  // static plugin interface
  static void * create(PF_ObjectParams *); 
  static apr_int32_t destroy(void *);
  ~StationarySatan();

  // IActor methods
  virtual void getInitialInfo(ActorInfo * info);
  virtual void play(ITurn * turnInfo);
  
private:
  StationarySatan();
};

#endif
