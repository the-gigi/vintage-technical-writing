#ifndef SERVICES_H
#define SERVICES_H

#include <apr-1/apr.h>

typedef struct LogParams
{
  const apr_byte_t * filename;
  apr_uint32_t       line;
  const apr_byte_t * message;
} LogParams;

typedef struct ReportErrorParams
{
  const apr_byte_t * filename;
  apr_uint32_t       line;
  const apr_byte_t * message;
} ReportErrorParams;

#endif
