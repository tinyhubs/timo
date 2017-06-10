#ifndef __timo_list_H_
#define __timo_list_H_

#include "timo_type.h"

typedef struct  timo_list
{
    struct timo_list*  next;
    struct timo_list*  prev;
}timo_list;

extern  timo_void   timo_list_init(timo_list*   list);

//  添加新元素
extern  timo_void   timo_list_attach(timo_list*   head,    timo_list*   newItem);

//  删除已有元素
extern  timo_list*  timo_list_detach(timo_list*   entry);

//  获取链表头的数据
extern  timo_list*  timo_list_top(timo_list*   head);

//  检查是否为空白链表
extern  timo_int    timo_list_empty(timo_list* head);

//  对链表进行遍历（不允许改变链表）
//  pos     timo_list*  
//  siblings    timo_list*  
#define timo_list_for_each(pos, siblings) \
    for (pos = (siblings)->next; pos != (siblings); pos = pos->next)

//  对链表进行遍历（允许改变链表）
//  pos     timo_list*  
//  n       timo_list*  
//  siblings    timo_list*  
#define timo_list_for_each_safe(pos, n, siblings) \
    for (pos = (siblings)->next, n = pos->next; pos != (siblings); pos = n, n = pos->next)

//  将遍历过程中的list转换成包含本链表的对象
//  type    第一个成员为timo_list类型的类型名
//  ptr     通过timo_list_for_each(_safe)宏找出来的对象的指针，对应着pos字段
//  member  type类型里面timo_list类型的成员的成员名称
#define timo_list_entry(type, ptr, member) \
    (type*)( ptr )

#endif//__timo_list_H_


