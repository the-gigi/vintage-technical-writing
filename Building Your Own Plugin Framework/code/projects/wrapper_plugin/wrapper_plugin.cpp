#include "wrapper_plugin.h"
#include "plugin_framework/plugin.h"
#include "plugin_framework/PluginHelper.h"
#include "GnarlyGolem.h"
#include "PsychicPiranea.h"

extern "C" PLUGIN_API 
PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params)
{
  PluginHelper p(params);
  p.registerObject<GnarlyGolem>((const apr_byte_t *)"GnarlyGolem");
  p.registerObject<PsychicPiranea>((const apr_byte_t *)"PsychicPiranea", PF_ProgrammingLanguage_CPP);

  return p.getResult();
}


