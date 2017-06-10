#ifndef __timo_tree_H_
#define __timo_tree_H_

#include "timo_type.h"
#include "timo_list.h"
#include "timo_map.h"

struct timo_tree
{
    timo_list   siblings;   //  兄弟节点(timo_list有约束，siblings必须是第一个节点)
    timo_list   childs;     //  子节点
    timo_tree*  parent;     //  父节点
};

extern  timo_tree*      timo_tree_new();
extern  timo_void       timo_tree_init(timo_tree* tree);
extern  timo_void       timo_tree_del(timo_tree* scope);
extern  timo_void       timo_tree_add_child(timo_tree* scope, timo_tree* newChild);


#endif//__timo_tree_H_

