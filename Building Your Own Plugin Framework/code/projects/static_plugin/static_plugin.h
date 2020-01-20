// cpp_plugin.cpp : Defines the entry point for the DLL application.
//


#include <plugin_framework/plugin.h>
#include "FidgetyPhantom.h"

extern "C" apr_int32_t StaticPlugin_ExitFunc();
extern "C" PF_ExitFunc StaticPlugin_InitPlugin(const PF_PlatformServices * params);
