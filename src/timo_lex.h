#ifndef __timo_lex_H_
#define __timo_lex_H_


#include "timo_type.h"
#include "timo_source.h"
#include "timo_token.h"
#include "timo_list.h"
#include "timo_buffer.h"

struct timo_lex;
extern timo_lex*    timo_lex_new(timo_int prefetchSize, timo_int limitSize, timo_int tokenInitSize);
extern timo_void    timo_lex_del(timo_lex* lex);
extern timo_void    timo_lex_push(timo_lex* lex, timo_source* source);
extern timo_void    timo_lex_pop(timo_lex* lex);
extern timo_int     timo_lex_next(timo_lex* lex);
extern timo_token*  timo_lex_token(timo_lex* lex);
extern timo_int     timo_lex_row(const notnull timo_lex* lex);
extern timo_int     timo_lex_col(const notnull timo_lex* lex);

#endif//__timo_lex_H_
