#include <hw/utils/base.hpp>
#include <hw/world/world.hpp>
#include <iostream>

int main(int argc, char** argv) 
{
  WorldProvider hp;
    
  std::string s = hp.getWorld();

  CHECK(s == "world"); 
  std::cout << s << std::endl;
  return 0;
}

