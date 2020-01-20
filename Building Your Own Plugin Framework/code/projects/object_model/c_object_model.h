#ifndef C_OBJECT_MODEL
#define C_OBJECT_MODEL

#include <apr-1/apr.h>

#define MAX_STR 64 /* max string length of string fields */

typedef struct C_ActorInfo_
{
  apr_uint32_t id;
  apr_byte_t   name[MAX_STR];
  apr_uint32_t location_x;
  apr_uint32_t location_y;
  apr_uint32_t health;
  apr_uint32_t attack;
  apr_uint32_t defense;
  apr_uint32_t damage;
  apr_uint32_t movement;
} C_ActorInfo;

typedef struct C_ActorInfoIteratorHandle_ { char c; } * C_ActorInfoIteratorHandle; 
typedef struct C_ActorInfoIterator_
{
  void (*reset)(C_ActorInfoIteratorHandle handle);
  C_ActorInfo * (*next)(C_ActorInfoIteratorHandle handle);

  C_ActorInfoIteratorHandle handle;
} C_ActorInfoIterator;

typedef struct C_TurnHandle_ { char c; } * C_TurnHandle; 
typedef struct C_Turn_
{
  C_ActorInfo * (*getSelfInfo)(C_TurnHandle handle);
  C_ActorInfoIterator * (*getFriends)(C_TurnHandle handle);
  C_ActorInfoIterator * (*getFoes)(C_TurnHandle handle);

  void (*move)(C_TurnHandle handle, apr_uint32_t x, apr_uint32_t y);
  void (*attack)(C_TurnHandle handle, apr_uint32_t id);

  C_TurnHandle handle;
} C_Turn;

typedef struct C_ActorHandle_ { char c; } * C_ActorHandle; 
typedef struct C_Actor_
{
  void (*getInitialInfo)(C_ActorHandle handle, C_ActorInfo * info);
  void (*play)(C_ActorHandle handle, C_Turn * turn); 

  C_ActorHandle handle;
} C_Actor;

typedef struct LogParmas
{
  const apr_byte_t * filename;
  apr_uint32_t       line;
  const apr_byte_t * message;
} LogParmas;

typedef struct ReportErrorParmas
{
  const apr_byte_t * filename;
  apr_uint32_t       line;
  const apr_byte_t * message;
} ReportErrorParmas;

#endif
