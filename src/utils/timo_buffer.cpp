#include "timo_type.h"
#include "timo_libc.h"
#include "timo_buffer.h"


timo_buffer* timo_buffer_new()
{
    timo_buffer*    imp         = (timo_buffer*)timo_malloc(sizeof(timo_buffer));

    timo_buffer_init(imp);

    return (imp);
}

timo_void    timo_buffer_init(timo_buffer* imp)
{
    if (TIMO_NULL == imp)
    {
        return;
    }

    imp->cap    = 0;
    imp->len    = 0;
    imp->buf    = TIMO_NULL;
}

timo_void    timo_buffer_clear(timo_buffer* buffer)
{
    if (TIMO_NULL == buffer)
    {
        return;
    }

    buffer->len    = 0;
}

timo_int     timo_buffer_is_empty(timo_buffer* buffer)
{
    if (TIMO_NULL == buffer)
    {
        return 1;
    }

    return ((buffer->len > 0) && (TIMO_NULL != buffer->buf));
}

timo_void    timo_buffer_del(timo_buffer* buffer)
{
    if (TIMO_NULL == buffer)
    {
        return;
    }

    if (TIMO_NULL != buffer->buf)
    {
        timo_free(buffer->buf);
    }

    timo_free(buffer);
    return;
}

timo_buffer* timo_buffer_of_cstr(const timo_char* str)
{
    //  创建一个新的字符串
    timo_buffer* newBuffer = (timo_buffer*)timo_buffer_new();
    if (TIMO_NULL == newBuffer)
    {
        return (TIMO_NULL);
    }

    //  按需分配内存
    int len = timo_strlen((TIMO_NULL == str)?"":str);
    newBuffer->buf = (timo_char*)timo_malloc(len + 1);

    //  将数据保存起来
    newBuffer->cap = len + 1;
    newBuffer->len = len;
    timo_memcpy(newBuffer->buf, str, len + 1);
    return ((timo_buffer*)newBuffer);
}

timo_char*   timo_buffer_to_cstr(timo_buffer* buffer)
{
    if (TIMO_NULL == buffer)
    {
        return (TIMO_NULL);
    }

    return (buffer->buf);
}

timo_void   timo_buffer_append(timo_buffer* buffer, const timo_char* data, timo_int size)
{
    if ((TIMO_NULL == buffer) || (TIMO_NULL == data) || (size <= 0))
    {
        return;
    }

    if ((buffer->len + size) > buffer->cap)
    {
        timo_int newSize = buffer->len + size + 1;
        timo_char* newBuf = new timo_char[newSize];
        timo_memcpy(newBuf, buffer->buf, buffer->len);
        timo_free(newBuf);
        buffer->buf = newBuf;
        buffer->cap = newSize;
    }

    timo_memcpy(buffer->buf + buffer->len, data, size);
}

