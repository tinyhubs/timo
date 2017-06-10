#ifndef __timo_array_H_
#define __timo_array_H_

#include "timo_type.h"

struct  timo_array
{
    timo_int    unit;   //  单元大小
    timo_int    len;    //  已经使用了多少个单元
    timo_int    cap;    //  总容量
    timo_void*  array;
};

extern  timo_array* timo_array_new(timo_int unit, timo_int initCap);
extern  timo_void   timo_array_init(timo_array* array);
extern  timo_void   timo_array_del(timo_array* array);
extern  timo_void   timo_array_expand(timo_array* array, timo_int count);
extern  timo_void   timo_array_append(timo_array* array, timo_void* data, timo_int count);

#define timo_array_for_each(pos, array)   \
     for (pos = (array)->array; \
          pos != (timo_char*)(array) + (array)->len * (array)->unit; \
          pos = (timo_char*)(pos) + (array)->len)

#define timo_array_entry(type,index,array) \
    (type)((timo_char*)(array->array) + array->unit * index)

#endif//__timo_array_H_
