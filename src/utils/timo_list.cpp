#include "timo_list.h"

//  将item插入prev与next之间
inline   timo_void  __timo_list_add(timo_list* item, timo_list* prev, timo_list* next)
{
    next->prev  =   item;
    item->next  =   next;
    item->prev  =   prev;
    prev->next  =   item;
}

//  将prev与next之间的项删除
inline   timo_void  __timo_list_del(timo_list* prev, timo_list* next)
{
    next->prev = prev;
    prev->next = next;
}

timo_void   timo_list_init(timo_list*   head)
{
    head->next  =   head;
    head->prev  =   head;
}

timo_void   timo_list_attach(timo_list*   head,    timo_list*   newItem)
{
    __timo_list_add(newItem, head, head->next);
}

timo_list*  timo_list_detach(timo_list*   entry)
{
    __timo_list_del(entry, entry->next);
    entry->prev =   entry;
    entry->next =   entry;
    return (entry);
}

timo_int    timo_list_empty(timo_list* head)
{
    return (head->next == head->prev);
}

timo_list*  timo_list_top(timo_list*   head)
{
    return (head->next);
}
