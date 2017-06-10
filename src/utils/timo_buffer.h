#ifndef __timo_buffer_H_
#define __timo_buffer_H_

#include "timo_type.h"

typedef struct 
{
    timo_int    cap;    //  容量
    timo_int    len;    //  数据的长度
    timo_char*  buf;    //  缓冲区
}timo_buffer;

extern timo_buffer* timo_buffer_new();
extern timo_void    timo_buffer_init(timo_buffer* buffer);
extern  timo_void   timo_buffer_clear(timo_buffer* buffer);
extern timo_void    timo_buffer_del(timo_buffer* buffer);
extern timo_buffer* timo_buffer_of_cstr(const timo_char* str);
extern timo_char*   timo_buffer_to_cstr(timo_buffer* buffer);
extern timo_int     timo_buffer_is_empty(timo_buffer* buffer);
extern timo_void    timo_buffer_append(timo_buffer* buffer, const timo_char* data, timo_int size);

#endif//__timo_buffer_H_
