#ifndef __timo_error_H_
#define __timo_error_H_

#include "timo_type.h"
#include "timo_libc.h"


#define TIMO_MAX_ERROR_SIZE 1024

typedef timo_int    timo_eid;
enum
{
#define  DEF_ERR(id,value,cn)   id = value, 
#include "timo_error_table.h"
#undef   DEF_ERR
};

extern timo_int         timo_error_formatv(timo_char* buffer, timo_int size, timo_int eid, va_list valist);
extern timo_int         timo_error_format(timo_char* buffer, timo_int size, timo_int eid, ...);
extern const timo_char* timo_error_get(timo_int eid);

#endif//__timo_error_H_
