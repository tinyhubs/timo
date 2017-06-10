#include "timo_map.h"
#include "timo_type.h"
#include "timo_libc.h"
#include "timo_list.h"

struct timo_pair
{
    timo_list   head;
    timo_void*  value;
    timo_char   key[1];
};


timo_int    timo_map_hash(const timo_char* string)
{
    timo_int hash = 5381;

    while (*string)
    {  
        hash = ((hash << 5) + hash) + *string;
        string++;
    }  

    return hash;  
}

timo_map*   timo_map_new()
{
    timo_map* map = (timo_map*)timo_malloc(sizeof(timo_map));
    timo_map_init(map);
    return (map);
}

timo_void   timo_map_init(timo_map* map)
{
    timo_list_init(&(map->pairs));
}

timo_void   timo_map_del(timo_map* map)
{
    if (TIMO_NULL == map)
    {
        return;
    }

    //  释放每一个元素
    timo_list* pos  =   TIMO_NULL;
    timo_list* next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(map->pairs))
    {
        timo_pair* pair = timo_list_entry(timo_pair, pos, head);
        timo_free(pair);
    }

    //  释放整个map
    timo_free(map);

    return;
}

timo_void*  timo_map_get(timo_map* map, timo_char* key)
{
    if (TIMO_NULL == map)
    {
        return (TIMO_NULL);
    }

    if (TIMO_NULL == key)
    {
        return (TIMO_NULL);
    }

    timo_list* pos  =   TIMO_NULL;
    timo_list* next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(map->pairs))
    {
        timo_pair* pair = timo_list_entry(timo_pair, pos, head);
        if (0 == timo_strcmp(pair->key, key))
        {
            return (pair->value);
        }
    }

    return (TIMO_NULL);
}

timo_void   timo_map_set(timo_map* map, timo_char* key, timo_void* object)
{
    if (TIMO_NULL == map)
    {
        return;
    }

    if (TIMO_NULL == key)
    {
        return;
    }

    timo_list* pos  =   TIMO_NULL;
    timo_list* next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(map->pairs))
    {
        timo_pair* pair = timo_list_entry(timo_pair, pos, head);
        if (0 == timo_strcmp(pair->key, key))
        {
            pair->value = object;
            return;
        }
    }

    timo_int len = timo_strlen(key);
    timo_pair*  pair = (timo_pair*)timo_malloc(sizeof(timo_pair) + len);
    timo_list_init(&(pair->head));
    timo_memcpy(pair->key, key, len);
    pair->key[len] = '\0';
    timo_list_attach(&(map->pairs), &(pair->head));

    return;
}


timo_void   timo_map_rmv(timo_map* map, timo_char* key)
{
    if (TIMO_NULL == map)
    {
        return;
    }

    if (TIMO_NULL == key)
    {
        return;
    }

    timo_list* pos  =   TIMO_NULL;
    timo_list* next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(map->pairs))
    {
        timo_pair* pair = timo_list_entry(timo_pair, pos, head);
        if (0 == timo_strcmp(pair->key, key))
        {
            timo_list_detach(&(pair->head));
            timo_free(pair);
            return;
        }
    }

    return;
}
