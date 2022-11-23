#include <iostream>
#include <algorithm>

char ROT13(char c)
{
  if (c >= 'a' && c < 'n')
    return char(int(c) + 13);
  else if (c > 'm' && c <= 'z')
    return char(int(c) - 13);
  else
    return c;
}

int main(int argc, char** argv) 
{
  std::string s("hello, world!");

  // Apply the secret ROT13 algorithm
  std::transform(s.begin(), s.end(), s.begin(), ROT13);
  std::cout << s.c_str() << std::endl;
  return 0;
}

