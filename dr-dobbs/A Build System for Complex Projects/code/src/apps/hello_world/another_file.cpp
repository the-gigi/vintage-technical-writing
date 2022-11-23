#include <hw/utils/base.hpp>
#include <hw/utils/DynamicLibrary.hpp>
#include <hw/hello/hello.hpp>
#include <hw/world/world.hpp>
#include <apps/hello_world/another_file.hpp>

#include <iostream>

// function pointer signature of getExclamationPoint
typedef char (*GetCharacterFunc)();

void go() 
{
  std::string errorString;
#ifdef WIN32
  std::string s("punctuator.dll");
#else
  std::string s("libpunctuator.dylib");
#endif
  
  DynamicLibrary * d = DynamicLibrary::load(s, errorString);
  CHECK(d)  << "error: " << errorString << d;
  
  void * sym = d->getSymbol("getExclamationPoint");
  CHECK(sym) << "Unable to get sumbol 'getExclamationPoint'";
  GetCharacterFunc getExclamationPoint = (GetCharacterFunc)sym;
  
  sym = d->getSymbol("getComma");
  CHECK(sym) << "Unable to get sumbol 'getComma'";
  GetCharacterFunc getComma = (GetCharacterFunc)sym;
  
  char exclamationPoint = getExclamationPoint();
  char comma = getComma();
  std::cout << HelloProvider().getHello()
            << comma
            << ' '
            << WorldProvider().getWorld()
            << exclamationPoint
            << std::endl;
  
  delete d;
}

