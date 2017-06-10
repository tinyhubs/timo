#ifndef __timo_synode_H_
#define __timo_synode_H_

#include "timo_type.h"

struct  timo_synode;
extern  timo_synode*    timo_synode_new(timo_void* symbol);
extern  timo_void       timo_synode_del(timo_synode* synode);
extern  timo_synode*    timo_synode_attach(timo_synode* synode, timo_void* symbol);
extern  timo_void*      timo_synode_detach(timo_synode* synode);

#endif//__timo_synode_H_

