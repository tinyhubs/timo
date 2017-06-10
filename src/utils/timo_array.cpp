#include "timo_array.h"
#include "timo_libc.h"

timo_array* timo_array_new(timo_int unit, timo_int initCap)
{
    timo_array* array = (timo_array*)timo_malloc(sizeof(timo_array));
    timo_array_init(array);
    timo_array_expand(array, initCap);
    return (array);
}


timo_void   timo_array_del(timo_array* array)
{
    if (TIMO_NULL == array)
    {
        return;
    }

    timo_free(array->array);
    timo_free(array);
}

timo_void   timo_array_init(timo_array* array)
{
    if (TIMO_NULL == array)
    {
        return;
    }

    array->unit =   0;
    array->len  =   0;
    array->cap  =   0;
    array->array = TIMO_NULL;
}

timo_void   timo_array_expand(timo_array* array, timo_int count)
{
    if (TIMO_NULL == array)
    {
        return;
    }

    if (count <= 0)
    {
        return;
    }

    timo_void* newArray = timo_malloc(array->unit * (array->cap + count));
    timo_memcpy(newArray, array->array, array->unit * array->len);
    timo_free(array->array);
    array->array = newArray;
    array->cap += count;
}

timo_void   timo_array_append(timo_array* array, timo_void* data, timo_int count)
{
    if (TIMO_NULL == array)
    {
        return;
    }

    if (TIMO_NULL == data)
    {
        return;
    }

    if (count < 0)
    {
        return;
    }

    timo_int expandCount = 0;
    if ((array->cap - array->len) < count)
    {
        expandCount = count - (array->cap - array->len);
    }

    timo_array_expand(array, expandCount);
    timo_memcpy((timo_char*)(array->array) + (array->unit * array->len), data, array->unit * array->len);
}




