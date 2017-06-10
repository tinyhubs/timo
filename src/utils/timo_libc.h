#ifndef __timo_libc_H_
#define __timo_libc_H_

#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "memory.h"
#include "string.h"

#define TIMO_NULL		NULL
#define TIMO_ASSERT		ASSERT


//@{    内存管理
#define	timo_malloc		malloc
#define timo_free		free
#define	timo_memcpy		memcpy
#define	timo_memset		memset
#define timo_memmove    memmove
//@}

//@{    控制台IO
#define	timo_vsnprintf	vsnprintf
#define timo_printf     printf
//@}

//@{    字符串操作
#define timo_strlen     strlen
#define timo_strcmp     strcmp
//@}

//@{    文件IO操作
#define timo_fopen      fopen
#define timo_fclose     fclose
#define timo_fread      fread
#define timo_fwrite     fwrite
#define timo_ferror     ferror
#define timo_feof       feof
//@}

//  成员函数的调用
#define CALL(_obj,_func,...)    (*((_obj)->self->p##_func))((_obj), __VA_ARGS__)

#endif//__timo_libc_H_
