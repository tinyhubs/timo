#ifndef __timo_scope_H_
#define __timo_scope_H_

#include "timo_type.h"

struct  timo_scope;
extern  timo_scope*     timo_scope_new();
extern  timo_void       timo_scope_del(timo_scope* scope);
extern  timo_scope*     timo_scope_parent(timo_scope* scope);
extern  timo_scope*     timo_scope_push(timo_scope* scope, timo_scope* newScope);
extern  timo_scope*     timo_scope_pop(timo_scope* scope);
extern  timo_scope*     timo_scope_def(timo_scope* scope, timo_void* symbol);
extern  timo_void*      timo_scope_get(timo_scope* scope, timo_char* symbolName);

#endif//__timo_scope_H_

