#include "hello.hpp"
#include <hw/letters/letters.hpp>

std::string HelloProvider::getHello()
{
  return get_h() +  get_e() + 
         get_l() + get_l() + get_o();
}

