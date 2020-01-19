#ifndef HW_BASE
#define HW_BASE

#include <hw/utils/StreamingException.hpp>

#define THROW throw StreamingException(__FILE__, __LINE__) \

#define CHECK(condition) if (!(condition)) \
  THROW << "CHECK FAILED: '" << #condition << "'"

#ifdef _DEBUG
  #define ASSERT(condition) if (!(condition)) \
     THROW << "ASSERT FAILED: '" << #condition << "'"
#else
  #define ASSERT(condition) {}
#endif // DEBUG


#endif // HW_BASE_H


