#include "apr-1/apr.h"
#include <utility>
#include <vector>

typedef std::pair<apr_uint32_t, apr_uint32_t> Position; 

double distance(Position p1, Position p2);

Position findClosest(Position p1, Position p2, apr_uint32_t movement);
