#ifndef __timo_symbol_H_
#define __timo_symbol_H_
#include "timo_type.h"
#include "timo_buffer.h"

struct timo_symbol;
struct timo_symbol_interface
{
    timo_void   (*pdel)(timo_symbol* symbol);
};

struct  timo_symbol
{
    timo_int    type;   //  ∑˚∫≈¿‡–Õ
    union
    {
        timo_buffer name;
    };

    timo_symbol_interface*  self;
};



extern  timo_symbol*    timo_symbol_new();
extern  timo_void       timo_symbol_del(timo_symbol* symbol);

#endif//__timo_symbol_H_

