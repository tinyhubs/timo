#ifndef __timo_hlist_H_
#define __timo_hlist_H_

struct  hlist_head
{
    struct hlist_node *first;
};

struct  hlist_node
{
    struct hlist_node *next,**pprev;
};

#define HLIST_HEAD_INIT { .first = NULL}
#define HLIST_HEAD(name) struct hlist_head name = {.first = NULL}
#endif//__timo_hlist_H_

