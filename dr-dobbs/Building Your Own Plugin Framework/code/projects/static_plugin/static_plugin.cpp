#include "plugin_framework/plugin.h"
#include "plugin_framework/PluginRegistrar.h"
#include "FidgetyPhantom.h"

extern "C" apr_int32_t StaticPlugin_ExitFunc()
{
  return 0;
}

extern "C" PF_ExitFunc StaticPlugin_InitPlugin(const PF_PlatformServices * params)
{
  int res = 0;
    
  PF_RegisterParams rp;
  rp.version.major = 1;
  rp.version.minor = 0;
  rp.programmingLanguage = PF_ProgrammingLanguage_CPP;
  
  // Regiater FidgetyPhantom
  rp.createFunc = FidgetyPhantom::create;
  rp.destroyFunc = FidgetyPhantom::destroy;
  res = params->registerObject((const apr_byte_t *)"FidgetyPhantom", &rp);
  if (res < 0)
    return NULL;

  return StaticPlugin_ExitFunc;
}

PluginRegistrar StaticPlugin_registrar(PluginRegistrar);
