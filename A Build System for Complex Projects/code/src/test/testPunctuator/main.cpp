#include <hw/utils/base.hpp>
#include <hw/utils/DynamicLibrary.hpp>
#include <iostream>

// function pointer signature of getExclamationPoint
typedef char (*GetCharacterFunc)();

int main(int argc, char** argv)
{
  if ((argc) != 2)
  {
    std::cout << "Usage: testPunctuator <extension: dll | dylib | so>"
              << std::endl;
    return 1;
  }

  std::string ext(argv[1]);
  std::string errorString;
#ifdef WIN32
  std::string s("punctuator.dll");
#else
  std::string s = std::string("libpunctuator.") + ext;
#endif


  DynamicLibrary * d = DynamicLibrary::load(s, errorString);
  CHECK(d) << "error: " << errorString << d;

  void * sym = d->getSymbol("getExclamationPoint");
  CHECK(sym) << "Unable to get sumbol 'getExclamationPoint'";

  GetCharacterFunc getExclamationPoint = (GetCharacterFunc)sym;

  char c = getExclamationPoint();
  CHECK(c == '!');
  std::cout << c << std::endl;

  delete d;
  return 0;
}
