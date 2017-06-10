#ifndef __timo_list_H_
#define __timo_list_H_

#include "timo_type.h"

typedef struct  timo_list
{
    struct timo_list*  next;
    struct timo_list*  prev;
}timo_list;

extern  timo_void   timo_list_init(timo_list*   list);

//  �����Ԫ��
extern  timo_void   timo_list_attach(timo_list*   head,    timo_list*   newItem);

//  ɾ������Ԫ��
extern  timo_list*  timo_list_detach(timo_list*   entry);

//  ��ȡ����ͷ������
extern  timo_list*  timo_list_top(timo_list*   head);

//  ����Ƿ�Ϊ�հ�����
extern  timo_int    timo_list_empty(timo_list* head);

//  ��������б�����������ı�����
//  pos     timo_list*  
//  siblings    timo_list*  
#define timo_list_for_each(pos, siblings) \
    for (pos = (siblings)->next; pos != (siblings); pos = pos->next)

//  ��������б���������ı�����
//  pos     timo_list*  
//  n       timo_list*  
//  siblings    timo_list*  
#define timo_list_for_each_safe(pos, n, siblings) \
    for (pos = (siblings)->next, n = pos->next; pos != (siblings); pos = n, n = pos->next)

//  �����������е�listת���ɰ���������Ķ���
//  type    ��һ����ԱΪtimo_list���͵�������
//  ptr     ͨ��timo_list_for_each(_safe)���ҳ����Ķ����ָ�룬��Ӧ��pos�ֶ�
//  member  type��������timo_list���͵ĳ�Ա�ĳ�Ա����
#define timo_list_entry(type, ptr, member) \
    (type*)( ptr )

#endif//__timo_list_H_


