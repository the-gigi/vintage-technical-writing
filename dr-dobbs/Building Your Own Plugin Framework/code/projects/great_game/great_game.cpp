#ifdef WIN32
#include "stdafx.h"
#endif

#include "plugin_framework/PluginManager.h"
#include "plugin_framework/Path.h"
#include "BattleManager.h"
#include "static_plugin/static_plugin.h"
#include <string>
#include <iostream>

using std::cout;
using std::endl;

apr_int32_t DummyInvokeService(const apr_byte_t * serviceName, void * serviceParams)
{
  return 0;
}

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main (int argc, char * argv[])
#endif
{
  cout << "Welcome to the great game!" << endl;
  if (argc != 2)
  {
    cout << "Usage: great_game <plugins dir>" << endl;
    return -1;
  }

  // Initialization
  ::apr_initialize();
  PluginManager & pm = PluginManager::getInstance();
  pm.getPlatformServices().invokeService = DummyInvokeService;
  pm.loadAll(Path::makeAbsolute(argv[1]));
  PluginManager::initializePlugin(StaticPlugin_InitPlugin);

  // Activate the battle manager
  BattleManager::getInstance().go();

  ::apr_terminate();

  return 0;
}

