#ifndef __timo_source_H_
#define __timo_source_H_

#include "timo_type.h"
typedef struct  timo_source
{
    timo_int    (*open)(struct timo_source* src);
    void        (*close)(struct timo_source* src);
    timo_int    (*read)(struct timo_source* src,  timo_void* buffer, timo_int bufferSize);
    timo_int    (*write)(struct timo_source* src, timo_void* data,   timo_int dataSize);
    timo_int    (*eof)(struct timo_source* src);
    timo_int    (*error)(struct timo_source* src);

}timo_source;


extern timo_source* timo_file_new(const timo_char* fileName, const timo_char* mode);
extern void         timo_file_del(timo_source* source);

#endif//__timo_source_H_
