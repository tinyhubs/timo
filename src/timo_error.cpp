#include "timo_error.h"
#include "timo_libc.h"

const timo_char*   timo_error_get(timo_int eid)
{
    const timo_char* fmt = "";
    switch (eid)
    {
#define  DEF_ERR(name,id,cn)    case name:  fmt = cn;   break;
#include "timo_error_table.h"
#undef   DEF_ERR
    }

    return (fmt);
}

timo_int timo_error_formatv(timo_char* buffer, timo_int size, timo_int eid, va_list valist)
{
    const timo_char* fmt = timo_error_get(eid);
    return (timo_vsnprintf(buffer, size, fmt, valist));
}

timo_int timo_error_format(timo_char* buffer, timo_int size, timo_int eid, ...)
{
    va_list valist;
    va_start(valist, eid);
    int msgSize = timo_error_formatv(buffer, size, eid, valist);
    va_end(valist);
    return (msgSize);
}

