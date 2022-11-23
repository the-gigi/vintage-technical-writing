/*
 * ----------------------------------------------------------------------
 *  Copyright (C) 2006,2007 Numenta Inc. All rights reserved.
 *
 *  The information and source code contained herein is the
 *  exclusive property of Numenta Inc. No part of this software
 *  may be used, reproduced, stored or distributed in any form,
 *  without explicit written authorization from Numenta Inc.
 * ----------------------------------------------------------------------
 */

/** @file 
 * ActorBaseTemplate<T, Interface>
 */

#ifndef PF_ACTOR_BASE_TEMPLATE
#define PF_ACTOR_BASE_TEMPLATE

#include "plugin_framework/plugin.h"
#include "plugin_framework/StreamingException.h"
#include "object_model.h"
#include "services.h"
#include "object_model_wrappers.h"

template <typename T, typename Interface=C_Actor>
class ActorBaseTemplate : 
  public C_Actor,
  public IActor
{
public:
  ActorBaseTemplate() : invokeService_(NULL)
  {
    // Initialize the function pointers of the C_Actor base class
    C_Actor::getInitialInfo = staticGetInitialInfo;
    C_Actor::play = staticPlay;
    C_Actor * handle = this;
    C_Actor::handle = (C_ActorHandle)handle;
  }

  virtual ~ActorBaseTemplate()
  {
  }

  // PF_CreateFunc from plugin.h
  static void * create(PF_ObjectParams * params)
  {
    T * actor = new T(params);
    // Set the error reporting function pointer
    actor->invokeService_ = params->platformServices->invokeService;
    
    // return the actor with the correct inerface
    return static_cast<Interface *>(actor);
  }
  
  // PF_DestroyFunc from plugin.h
  static apr_int32_t destroy(void * actor)
  {
    if (!actor)
      return -1;
    delete ActorBaseTemplate<T, Interface>::getSelf(reinterpret_cast<Interface *>(actor));
    return 0;
  }
   
  // Helper method to convert the C_Actor * argument 
  // in every method to an ActorBaseTemplate<T, Interface> instance pointer
  static ActorBaseTemplate<T, Interface> * getSelf(C_Actor * actor)
  {
    return static_cast<ActorBaseTemplate<T, Interface> *>(actor);
  }

  static ActorBaseTemplate<T, Interface> * getSelf(IActor * actor)
  {
    return static_cast<ActorBaseTemplate<T, Interface> *>(actor);
  }

  static ActorBaseTemplate<T, Interface> * getSelf(C_ActorHandle handle)
  {
    return static_cast<ActorBaseTemplate<T, Interface> *>((C_Actor *)handle);
  }

  // Helper method to report errors from a static function
  static void reportError(C_ActorHandle handle, 
                          const apr_byte_t * filename, 
                          apr_uint32_t line, 
                          const apr_byte_t * message)
  {
    ActorBaseTemplate<T, Interface> * self = ActorBaseTemplate<T, Interface>::getSelf(handle);
    ReportErrorParams rep;
    rep.filename = filename;
    rep.line = line;
    rep.message = message;
    self->invokeService_((const apr_byte_t *)"reportError", &rep);
  }
 
  // C_Actor functions
  static void staticGetInitialInfo(C_ActorHandle handle, C_ActorInfo * info)
  {

    ActorBaseTemplate<T, Interface> * self = ActorBaseTemplate<T, Interface>::getSelf(handle);
    try
    {
      self->getInitialInfo(info);
    }
    catch (const StreamingException & e)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)e.filename_.c_str(), e.line_, (const apr_byte_t *)e.what());
    }
    catch (const std::runtime_error & e)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)__FILE__, __LINE__, (const apr_byte_t *)e.what());
    }      
    catch (...)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)__FILE__, __LINE__, (const apr_byte_t *)"ActorBaseTemplate<T, Interface>::staticGetInitialInfo() failed");
    }
  }
  
  static void staticPlay(C_ActorHandle handle, C_Turn * turn)
  {
    try
    {
      TurnWrapper tw(turn);
      getSelf((C_Actor *)handle)->play(&tw);
    }
    catch (const StreamingException & e)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)e.filename_.c_str(), e.line_, (const apr_byte_t *)e.what());
    }
    catch (const std::runtime_error & e)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)__FILE__, __LINE__, (const apr_byte_t *)e.what());
    }      
    catch (...)
    {
      ActorBaseTemplate<T, Interface>::reportError(handle, (const apr_byte_t *)__FILE__, __LINE__, (const apr_byte_t *)"ActorBaseTemplate<T, Interface>::staticPlay() failed");
    }
  }
 
  // Empty implementation of IActor
  virtual void getInitialInfo(ActorInfo * info)
  {
  }

  virtual void play(ITurn * turnInfo)
  {
  }
  
private:
  PF_InvokeServiceFunc invokeService_;
};

#endif
