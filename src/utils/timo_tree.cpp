#include "timo_type.h"
#include "timo_tree.h"
#include "timo_libc.h"

timo_tree* timo_tree_new()
{
    timo_tree* scope = (timo_tree*)timo_malloc(sizeof(timo_tree));
    timo_tree_init(scope);
    return (scope);
}

timo_void       timo_tree_init(timo_tree* tree)
{
    timo_list_init(&(tree->siblings));
    timo_list_init(&(tree->childs));
    tree->parent  = TIMO_NULL;
}

timo_void   timo_tree_del(timo_tree* scope)
{
    if (TIMO_NULL == scope)
    {
        return;
    }

    //  释放所有的子作用域
    timo_list* pos  = TIMO_NULL;
    timo_list* next = TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(scope->childs))
    {
        timo_tree* childScope = timo_list_entry(timo_tree, pos, head);
        timo_tree_del(childScope);
    }

    //  将自己从父节点中摘除
    timo_list_detach(&(scope->siblings));

    timo_free(scope);
}

timo_void   timo_tree_add_child(timo_tree* scope, timo_tree* newChild)
{
    if (TIMO_NULL == scope)
    {
        return;
    }

    if (TIMO_NULL == newChild)
    {
        return;
    }

    timo_list_attach(&(scope->childs), &(newChild->siblings));
    newChild->parent = scope;
}
