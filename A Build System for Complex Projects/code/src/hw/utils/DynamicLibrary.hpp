
#ifndef HW_DYNAMIC_LIBRARY_HPP
#define HW_DYNAMIC_LIBRARY_HPP

//----------------------------------------------------------------------

#ifdef WIN32
  #include <Windows.h>
#else
  #include <dlfcn.h>
#endif

#include <string>

class DynamicLibrary
{
public:
  enum Mode
  {
    #ifdef WIN32
    LAZY,
    GLOBAL,
    LOCAL,
    NOW
    #else
    LAZY = RTLD_LAZY,
    GLOBAL = RTLD_GLOBAL,
    LOCAL = RTLD_LOCAL,
    NOW = RTLD_NOW
    #endif
  };

  static DynamicLibrary * load(const std::string & path, 
                               int mode, 
                               std::string &errorString);

  static DynamicLibrary * load(const std::string & path, 
                               std::string &errorString);
  ~DynamicLibrary();

  void * getSymbol(const std::string & name);

private:
  DynamicLibrary();

  DynamicLibrary(void * handle);
  DynamicLibrary(const DynamicLibrary &);

private:
  void * handle_;  
};


#endif

