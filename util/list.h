#ifndef LIST_H
#define LIST_H

#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>

struct list {
    struct list *next, *prev;
};

static inline void list_init(struct list *list) {
    list->next = list;
    list->prev = list;
}

static inline void _list_add_between(struct list *prev, struct list *next, struct list *item) {
    prev->next = item;
    item->prev = prev;
    item->next = next;
    next->prev = item;
}

static inline void list_add(struct list *list, struct list *item) {
    _list_add_between(list, list->next, item);
}

static inline void list_remove(struct list *item) {
    item->prev->next = item->next;
    item->next->prev = item->prev;
    item->next = item->prev = NULL;
}

static inline bool list_empty(struct list *list) {
    return list->next == list || (list->next == NULL && list->prev == NULL);
}

#define list_entry(item, type, member) \
    (type *) ((char *) (item) - offsetof(type, member))
#define list_first_entry(list, type, member) \
    list_entry((list)->next, type, member)
#define list_next_entry(item, member) \
    list_entry((item)->member.next, typeof(*(item)), member)

#define list_for_each(list, item) \
    for (item = (list)->next; item != (list); item = item->next)
#define list_for_each_safe(list, item, tmp) \
    for (item = (list)->next, tmp = item->next; item != (list); \
            item = tmp, tmp = item->next)

#define list_for_each_entry(list, item, member) \
    for (item = list_entry((list)->next, typeof(*item), member); \
            &item->member != (list); \
            item = list_entry(item->member.next, typeof(*item), member))
#define list_for_each_entry_safe(list, item, tmp, member) \
    for (item = list_first_entry(list, typeof(*(item)), member), \
            tmp = list_next_entry(item, member); \
            &item->member != (list); \
            item = tmp, tmp = list_next_entry(item, member))

static inline int list_size(struct list *list) {
    int count = 0;
    struct list *item;
    list_for_each(list, item) {
        count++;
    }
    return count;
}

#endif
