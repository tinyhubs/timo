#ifndef __timo_tree_H_
#define __timo_tree_H_

#include "timo_type.h"
#include "timo_list.h"
#include "timo_map.h"

struct timo_tree
{
    timo_list   siblings;   //  �ֵܽڵ�(timo_list��Լ����siblings�����ǵ�һ���ڵ�)
    timo_list   childs;     //  �ӽڵ�
    timo_tree*  parent;     //  ���ڵ�
};

extern  timo_tree*      timo_tree_new();
extern  timo_void       timo_tree_init(timo_tree* tree);
extern  timo_void       timo_tree_del(timo_tree* scope);
extern  timo_void       timo_tree_add_child(timo_tree* scope, timo_tree* newChild);


#endif//__timo_tree_H_

