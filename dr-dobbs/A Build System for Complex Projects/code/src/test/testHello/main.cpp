#include <hw/utils/base.hpp>
#include <hw/hello/hello.hpp>
#include <iostream>

int main(int argc, char** argv) 
{
  HelloProvider hp;
    
  std::string s = hp.getHello();

  CHECK(s == "hello");
  std::cout << s << std::endl;
  return 0;
}

