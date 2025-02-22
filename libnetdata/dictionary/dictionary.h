// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETDATA_DICTIONARY_H
#define NETDATA_DICTIONARY_H 1

#include "../libnetdata.h"

struct dictionary_stats {
    unsigned long long inserts;
    unsigned long long deletes;
    unsigned long long searches;
    unsigned long long entries;
};

typedef struct name_value {
    avl_t avl_node;         // the index - this has to be first!

    uint32_t hash;          // a simple hash to speed up searching
                            // we first compare hashes, and only if the hashes are equal we do string comparisons

    char *name;
    void *value;
} NAME_VALUE;

typedef struct dictionary {
    avl_tree_type values_index;

    uint8_t flags;

    struct dictionary_stats *stats;
    netdata_rwlock_t *rwlock;
} DICTIONARY;

#define DICTIONARY_FLAG_NONE                    0x00000000
#define DICTIONARY_FLAG_SINGLE_THREADED         0x00000001
#define DICTIONARY_FLAG_VALUE_LINK_DONT_CLONE   0x00000002
#define DICTIONARY_FLAG_NAME_LINK_DONT_CLONE    0x00000004
#define DICTIONARY_FLAG_WITH_STATISTICS         0x00000008
#define DICTIONARY_FLAG_DONT_OVERWRITE_VALUE    0x00000010

extern DICTIONARY *dictionary_create(uint8_t flags);
extern void dictionary_destroy(DICTIONARY *dict);
extern void *dictionary_set_with_name_ptr(DICTIONARY *dict, const char *name, void *value, size_t value_len, char **name_ptr) NEVERNULL;
#define dictionary_set(dict, name, value, value_len) dictionary_set_with_name_ptr(dict, name, value, value_len, NULL)
extern void *dictionary_get(DICTIONARY *dict, const char *name);
extern int dictionary_del(DICTIONARY *dict, const char *name);

extern int dictionary_get_all(DICTIONARY *dict, int (*callback)(void *entry, void *d), void *data);
extern int dictionary_get_all_name_value(DICTIONARY *dict, int (*callback)(char *name, void *entry, void *d), void *data);

#endif /* NETDATA_DICTIONARY_H */
