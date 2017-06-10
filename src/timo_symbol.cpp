#include "timo_symbol.h"
#include "timo_libc.h"

timo_symbol*    timo_symbol_new()
{
    timo_symbol* synode = (timo_symbol*)(timo_malloc(sizeof(timo_symbol)));
    return (synode);
}

timo_void       timo_symbol_del(timo_symbol* symbol)
{
    if (TIMO_NULL == symbol)
    {
        return;
    }

    timo_free(symbol);
}

