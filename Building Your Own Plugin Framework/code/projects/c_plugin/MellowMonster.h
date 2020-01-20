#ifndef MELLOW_MONSTER_H
#define MELLOW_MONSTER_H

#include <plugin_framework/plugin.h>

// static plugin interface
void * MellowMonster_create(PF_ObjectParams *); 
apr_int32_t MellowMonster_destroy(void *);

#endif

