
#ifdef WIN32
#include "stdafx.h"
#endif

#include "c_plugin.h"

// cpp_plugin.cpp : Defines the entry point for the DLL application.
//

#include "c_plugin.h"
#include "plugin_framework/plugin.h"
#include "MellowMonster.h"

PLUGIN_API apr_int32_t ExitFunc()
{
  return 0;
}

PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params)
{
  int res = 0;
    
  PF_RegisterParams rp;
  rp.version.major = 1;
  rp.version.minor = 0;
  
  // Regiater MellowMonster
  rp.createFunc = MellowMonster_create;
  rp.destroyFunc = MellowMonster_destroy;
  rp.programmingLanguage = PF_ProgrammingLanguage_C;
  
  res = params->registerObject((const apr_byte_t *)"MellowMonster", &rp);
  if (res < 0)
    return NULL;

  return ExitFunc;
}



