#ifndef __timo_map_H_
#define __timo_map_H_

#include "timo_type.h"
#include "timo_list.h"

struct timo_map
{
    timo_list   pairs;
};

extern  timo_map*   timo_map_new();
extern  timo_void   timo_map_del(timo_map* map);
extern  timo_void   timo_map_init(timo_map* map);
extern  timo_void*  timo_map_get(timo_map* map, timo_char* key);
extern  timo_void   timo_map_set(timo_map* map, timo_char* key, timo_void* object);
extern  timo_void   timo_map_rmv(timo_map* map, timo_char* key);

#define timo_map_for_each(pos, siblings) \
    for (pos = (siblings)->pairs.next; pos != &((siblings)->pairs); pos = pos->next)

#define timo_map_for_each_safe(pos, n, siblings) \
    for (pos = (siblings)->pairs->next, n = pos->next; pos != &((siblings)->pairs); pos = n, n = pos->next)

#define timo_map_entry(type, ptr, member) \
    (type*)( ptr )

#endif//__timo_map_H_


