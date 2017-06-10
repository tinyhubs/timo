#include "timo_source.h"
#include "timo_type.h"
#include "timo_buffer.h"
#include "timo_libc.h"
#include "timo_error.h"



typedef struct timo_source_file
{
    timo_source     prototype;
    timo_buffer*    name;
    FILE*           file;
}timo_source_file;



static timo_int    timo_file_open(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (-1);
    }

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL == source->file)
    {
        return (-2);
    }

    if (TIMO_NULL == source->name)
    {
        return (-3);
    }

    source->file = timo_fopen(timo_buffer_to_cstr(source->name), "r+");
    if (TIMO_NULL == source->file)
    {
        return (ERR_OPEN_SRC_FILE_FAILED);
    }

    return (0);
}

static void        timo_file_close(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return;
    }

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL != source->file)
    {
        timo_fclose(source->file);
        source->file = TIMO_NULL;
        return;
    }

    if (TIMO_NULL == source->name)
    {
        timo_buffer_del(source->name);
        source->name = TIMO_NULL;
        return;
    }
}

static timo_int    timo_file_read(timo_source* src,  timo_void* buffer, timo_int bufferSize)
{
    if (TIMO_NULL == src)
    {
        return (0);
    }

    if (TIMO_NULL == buffer)
    {
        return (0);
    }

    if (TIMO_NULL == bufferSize)
    {
        return (0);
    }

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL == source->file)
    {
        return (0);
    }

    return (timo_fread(buffer, 1, bufferSize, source->file));
}

static timo_int    timo_file_append(timo_source* src, timo_void* data,   timo_int dataSize)
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

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL == source->file)
    {
        return (0);
    }

    return (timo_fwrite(data, 1, dataSize, source->file));
}

static timo_int    timo_file_eof(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (1);
    }

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL == source->file)
    {
        return (1);
    }

    return (timo_feof(source->file));
}

static timo_int    timo_file_error(timo_source* src)
{
    if (TIMO_NULL == src)
    {
        return (1);
    }

    timo_source_file* source = (timo_source_file*)src;
    if (TIMO_NULL == source->file)
    {
        return (1);
    }

    return (timo_ferror(source->file));
}

static timo_source timo_file_type()
{
    timo_source type = {TIMO_NULL};
    type.open      =   timo_file_open;
    type.close     =   timo_file_close;
    type.read      =   timo_file_read;
    type.write    =   timo_file_append;
    type.eof       =   timo_file_eof;
    type.error     =   timo_file_error;
    return (type);
}

/////////////////////////////////
timo_source* timo_file_new(const timo_char* fileName, const timo_char* mode)
{
    if ((TIMO_NULL == fileName) || (TIMO_NULL == mode))
    {
        return (TIMO_NULL);
    }

    timo_source_file* source = (timo_source_file*)timo_malloc(sizeof(timo_source_file));
    if (TIMO_NULL == source)
    {
        return ((timo_source*)source);
    }

    source->name = timo_buffer_of_cstr(fileName);
    source->file = timo_fopen(fileName, mode);
    source->prototype = timo_file_type();
    if ((TIMO_NULL == source->name) || (TIMO_NULL == source->file))
    {
        goto    fail;
    }

    return ((timo_source*)source);

fail:
    if (TIMO_NULL != source->name)
    {
        timo_buffer_del(source->name);
    }

    if (TIMO_NULL != source->file)
    {
        timo_fclose(source->file);
    }

    return (TIMO_NULL);
}


void         timo_file_del(timo_source* source)
{

}
