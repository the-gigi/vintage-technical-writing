
#ifndef FIDGETY_PHANTOM
#define FIDGETY_PHANTOM

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <object_model/object_model.h>

struct PF_ObjectParams;

class FidgetyPhantom : public IActor
{
public:

  // static plugin interface
  static void * create(PF_ObjectParams *); 
  static apr_int32_t destroy(void *);
  ~FidgetyPhantom();

  // IActor methods
  virtual void getInitialInfo(ActorInfo * info);
  virtual void play(ITurn * turnInfo);
  
private:
  FidgetyPhantom();
};

#endif
