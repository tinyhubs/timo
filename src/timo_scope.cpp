#include "timo_scope.h"

#include "timo_tree.h"
#include "timo_libc.h"

//  ������ڵ�
struct timo_scope
{
    timo_tree       head;
    timo_map        symbols;
};

//  �����µ�������
timo_scope*     timo_scope_new()
{
    timo_scope* scope = (timo_scope*)timo_malloc(sizeof(timo_scope));
    timo_tree_init(&(scope->head));
    timo_map_init(&(scope->symbols));
    return (scope);
}

//  ɾ��һ��������
timo_void       timo_scope_del(timo_scope* scope)
{
    if (TIMO_NULL == scope)
    {
        return;
    }

    //  �ݹ�ɾ�����е��ֵܽڵ�
    timo_tree_del(&(scope->head));

    //  ɾ��һ���������е����еķ���
    timo_map_del(&(scope->symbols));

    //  ɾ������������
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

