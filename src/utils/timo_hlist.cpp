static inline void INIT_HLIST_NODE(struct hlist_node *h)
{
    h->next = NULL;
    h->pprev = NULL;
}

static inline  int hlist_empty(const struct hlist_head *h)
{
    return !h->first;
}

static inline int hlist_unhashed(const struct hlist_node *h)
{
    return !h->pprev
}

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node  *next =  n->next;  
    struct hlist_node  **pprev = n->pprev;
    *pprev = next;
    if (next)
        next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = LIST_POISON1;
    n->pprev = LIST_POISON2;
}

static inline void hlist_del_init(struct hlist_node *n)
{
    if (!hlist_unhashed(n)) {
        __hlist_del(n);
        INIT_HLIST_NODE(n);
    }
}

static inline void hlist_add_head(struct hlist_node *n,struct hlist_head *h)
{
    struct  hlist_node  *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first; 
}
/*next must be !=NULL*/
static inline void hlist_add_before(struct hlist_node *n, struct hlist_node *next)
{
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    *(n->pprev) =n ;
}

static inline void hlist_add_after(struct hlist_node *n, struct hlist_node *next )
{
    next->next = n->next;
    n->next = next;
    next->pprev = &n->next;
    if (next->next)
        next->next->pprev = &next->next;
} 


static inline void hlist_add_fake(struct hlist_node  *n)
{
    n->pprev =&n->next;
}

static inline void hlist_move_list(struct hlist_head *old, struct hlist_head *new)
{
    new->first = old->first;
    if (new->first)
        new->fist->pprev = &new->first;
    old->first = NULL;
}

#define hlist_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define hlist_for_each(pos, head)          \
    for(pos = (head)->first; pos ; pos = pos->next)

#define  hlist_for_each_safe(pos,n,head)    \
    for(pos = (head)->first,pos &&({n=pos->next;1;}) ; pos=n)


#define hlist_for_each_entry(tpos,pos,head,member)                   \
    for (pos = (head)->first;                                    \
    pos &&                                                  \
    ({tpos = hlist_entry(pos, typeof(*tpos),member);1;});\
    pos = pos->next)     

#define hlist_for_each_entry_safe(tpos, pos, n, head, member)         \
    for (pos = (head)->first;                                     \
    pos && ({ n = pos->next;1;})  &&                         \
    ({tpos = hlist_entry(pos, typeof(*tpos),member);1;});    \
    pos = n)   

#define  hlist_for_each_entry_continue(tpos, pos, member)                \
    for (pos = (pos)->next;                                     \
    pos &&                                                \
    ({tpos = hlist_entry(pos,typeof(*tpos),member);1;});    \
    pos = pos->next)

#define hlist_for_each_entry_from(tpos, pos, member)                \
    for (; pos &&                                               \
    ({tpos = hlist_entry(pos,typeof(*tpos),member);1;});\
    pos = pos->next)