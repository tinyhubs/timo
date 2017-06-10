#include "timo_scope.h"

#include "timo_tree.h"
#include "timo_libc.h"

//  作用域节点
struct timo_scope
{
    timo_tree       head;
    timo_map        symbols;
};

//  创建新的作用域
timo_scope*     timo_scope_new()
{
    timo_scope* scope = (timo_scope*)timo_malloc(sizeof(timo_scope));
    timo_tree_init(&(scope->head));
    timo_map_init(&(scope->symbols));
    return (scope);
}

//  删除一个作用域
timo_void       timo_scope_del(timo_scope* scope)
{
    if (TIMO_NULL == scope)
    {
        return;
    }

    //  递归删除所有的兄弟节点
    timo_tree_del(&(scope->head));

    //  删除一个作用域中的所有的符号
    timo_map_del(&(scope->symbols));

    //  删除作用域自身
    timo_free(scope);
}

timo_scope*     timo_scope_parent(timo_scope* scope)
{
    return ((timo_scope*)(scope->head.parent));
}

timo_scope*     timo_scope_push(timo_scope* scope, timo_scope* newScope)
{
    timo_tree_add_child(&(scope->head), (timo_tree*)(newScope));
    return (newScope);
}

timo_scope*     timo_scope_pop(timo_scope* scope)
{
    return (timo_scope_parent(scope));
}

timo_scope*     timo_scope_def(timo_scope* scope, timo_char* symbolName, timo_void* symbol)
{
    timo_map_set(&(scope->symbols), symbolName, symbol);
    return (scope);
}

timo_void*      timo_scope_get(timo_scope* scope, timo_char* symbolName)
{
    return (timo_map_get((timo_map*)scope, symbolName));
}

