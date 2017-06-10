#include "timo_synode.h"
#include "timo_tree.h"
#include "timo_symbol.h"
#include "timo_libc.h"
#include "timo_type.h"

struct timo_synode;

struct timo_synode_interface
{
    timo_void   (*pdel)(timo_synode* symbol);
};

//  语法节点
struct timo_synode
{
    timo_tree   head;
    timo_void*  symbol;     //  关联的一个符号
    timo_synode_interface*  self;
};

timo_synode*   timo_synode_new(timo_void* symbol)
{
    timo_synode* synode = (timo_synode*)(timo_malloc(sizeof(timo_synode)));
    timo_tree_init(&(synode->head));
    synode->symbol = symbol;
    return (synode);
}

timo_void      timo_synode_del(timo_synode* synode)
{
    if (TIMO_NULL == synode)
    {
        return;
    }

    timo_symbol_del((timo_symbol*)(synode->symbol));
    timo_free(synode);
}

timo_synode*    timo_synode_attach(timo_synode* synode, timo_void* symbol)
{
    if (TIMO_NULL == synode)
    {
        return (TIMO_NULL);
    }

    synode->symbol = symbol;
    return (synode);
}

timo_void*      timo_synode_detach(timo_synode* synode)
{
    if (TIMO_NULL == synode)
    {
        return (TIMO_NULL);
    }

    timo_void* symbol = synode->symbol;
    synode->symbol = TIMO_NULL;
    return (symbol);
}

