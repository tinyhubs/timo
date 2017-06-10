#include "timo_source.h"
#include "timo_type.h"
#include "timo_buffer.h"
#include "timo_libc.h"
#include "timo_error.h"


enum    
{
    READ_MODE,
    WRITE_MODE
};

typedef struct timo_source_string
{
    timo_source     prototype;
    timo_buffer     string;
    timo_char*      cp;
    timo_int        error;
    timo_int        mode;
}timo_source_string;



static timo_int    timo_string_open(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (-1);
    }

    timo_source_string* source = (timo_source_string*)src;
    if (TIMO_NULL != source->cp)
    {
        return (-2);    ///<    已经初始化了
    }

    if (timo_buffer_is_empty(&(source->string)))
    {
        return (-3);
    }

    source->cp    = source->string.buf;
    source->error = 0;

    return (0);
}

static void        timo_string_close(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return;
    }

    timo_source_string* source = (timo_source_string*)src;
    timo_buffer_clear(&(source->string));
    source->cp    = TIMO_NULL;
    source->error = 0;
}

static timo_int    timo_string_read(timo_source* src,  timo_void* buffer, timo_int bufferSize)
{
    if (TIMO_NULL == buffer)
    {
        return (0);
    }

    if (bufferSize <= 0)
    {
        return (0);
    }

    if (TIMO_NULL == src)
    {
        return (0);
    }

    //  如果遇到错误
    timo_source_string* source = (timo_source_string*)src;
    if (0 != source->error)
    {
        return (-1);
    }

    //  读取指针为NULL，说明还未打开
    if (TIMO_NULL == source->cp)
    {
        return (-2);
    }

    //  如果缓冲区为空，返回读取的数据的长度为0
    if (timo_buffer_is_empty(&(source->string)))
    {
        return (0);
    }

    //  计算最多可以拷贝多长的数据
    timo_int readSize = (source->string.buf + source->string.len) - source->cp;
    if (readSize < bufferSize)
    {
        readSize = bufferSize;
    }

    timo_memcpy(buffer, source->cp, readSize);

    return (readSize);
}

static timo_int    timo_string_write(timo_source* src, timo_void* data,   timo_int dataSize)
{
    if (TIMO_NULL == src)
    {
        return (0);
    }

    if (TIMO_NULL == data)
    {
        return (0);
    }

    if (dataSize <= 0)
    {
        return (0);
    }

    timo_source_string* source = (timo_source_string*)src;
    timo_buffer_append(&(source->string), (timo_char*)data, dataSize);

    return (dataSize);
}

static timo_int    timo_string_eos(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (1);
    }

    timo_source_string* source = (timo_source_string*)src;

    //  写模式永远不会到达结尾
    if (WRITE_MODE == source->mode)
    {
        return (0);
    }

    if (timo_buffer_is_empty(&(source->string)))
    {
        return (1);
    }

    if (TIMO_NULL == source->cp)
    {
        return (1);
    }

    return (0);
}

static timo_int    timo_string_error(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (1);
    }

    timo_source_string* source = (timo_source_string*)src;
    if (TIMO_NULL == source->cp)
    {
        return (1);
    }

    if (0 != source->error)
    {
        return (1);
    }

    return (0);
}

static timo_source timo_string_type()
{
    timo_source type = {TIMO_NULL};
    type.open      =   timo_string_open;
    type.close     =   timo_string_close;
    type.read      =   timo_string_read;
    type.write     =   timo_string_write;
    type.eof       =   timo_string_eos;
    type.error     =   timo_string_error;
    return (type);
}

/////////////////////////////////
timo_source* timo_string_new_read(const timo_char* string, timo_int size)
{
    if ((TIMO_NULL == string) || (size <= 0))
    {
        return (TIMO_NULL);
    }

    timo_source_string* source = (timo_source_string*)timo_malloc(sizeof(timo_source_string));
    if (TIMO_NULL == source)
    {
        return ((timo_source*)source);
    }

    source->prototype = timo_string_type();
    timo_buffer_init(&(source->string));
    timo_buffer_append(&(source->string), string, size);
    source->cp = source->string.buf;
    source->error = 0;
    source->mode = READ_MODE;

    return ((timo_source*)source);
}


timo_source* timo_string_new_write()
{
    timo_source_string* source = (timo_source_string*)timo_malloc(sizeof(timo_source_string));
    if (TIMO_NULL == source)
    {
        return ((timo_source*)source);
    }

    source->prototype = timo_string_type();
    timo_buffer_init(&(source->string));
    source->cp = source->string.buf;
    source->error = 0;
    source->mode = WRITE_MODE;

    return ((timo_source*)source);
}


void         timo_string_del(timo_source* source)
{

}
