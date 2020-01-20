#include "cpp_plugin.h"
#include "plugin_framework/plugin.h"
#include "KillerBunny.h"
#include "StationarySatan.h"

extern "C" PLUGIN_API apr_int32_t ExitFunc()
{
  return 0;
}

extern "C" PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params)
{
  int res = 0;
    
  PF_RegisterParams rp;
  rp.version.major = 1;
  rp.version.minor = 0;
  rp.programmingLanguage = PF_ProgrammingLanguage_CPP;
  
  // Regiater KillerBunny
  rp.createFunc = KillerBunny::create;
  rp.destroyFunc = KillerBunny::destroy;
  res = params->registerObject((const apr_byte_t *)"KillerBunny", &rp);
  if (res < 0)
    return NULL;

  // Regiater StationarySatan
  rp.createFunc = StationarySatan::create;
  rp.destroyFunc = StationarySatan::destroy;
  res = params->registerObject((const apr_byte_t *)"StationarySatan", &rp);
  if (res < 0)
    return NULL;

  return ExitFunc;
}


