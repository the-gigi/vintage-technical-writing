#include "utils.h"
#include <math.h>

double calcDistance(Position p1, Position p2)
{
  return ::sqrt((double)(p2.second - p1.second) * (p2.second - p1.second) + (p2.first - p1.first) * (p2.first - p1.first));
}

Position findClosest(Position p1, Position p2, apr_uint32_t movement)
{
  double d = calcDistance(p1, p2);
  if (d <= movement)
    return p2;

  double ratio = movement / d;

  apr_int32_t dx = apr_int32_t((p2.first - p1.first) * ratio);
  apr_int32_t dy = apr_int32_t((p2.second - p1.second) * ratio);

  return Position(p1.first + dx, p1.second + dy);
}

