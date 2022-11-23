#include "world.hpp"
#include <hw/letters/letters.hpp>

std::string WorldProvider::getWorld()
{
  return get_w() + get_o() + 
         get_r() + get_l() + get_d();
}

