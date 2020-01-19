#include <hw/utils/base.hpp>
#include <hw/letters/letters.hpp>
#include <iostream>

int main(int argc, char** argv) 
{
  CHECK(get_h() == std::string("h")); 
  CHECK(get_e() == std::string("e")); 
  CHECK(get_l() == std::string("l")); 
  CHECK(get_o() == std::string("o")); 
  CHECK(get_w() == std::string("w")); 
  CHECK(get_r() == std::string("r")); 
  CHECK(get_d() == std::string("d")); 
  
  return 0;
}

