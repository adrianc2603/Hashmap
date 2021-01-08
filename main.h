#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t {
    void *key;
    void *value;  
    struct node_t *next; 
} node_t;

typedef struct hashmap_t {
    node_t **entries;
    int capacity;
    int num_entries;
    size_t (*hash)(void*); // Pointer to the hash function
    int (*cmp)(void*, void*); // Pointer to the compare function 
} hashmap_t;

hashmap_t *create_hashmap(size_t (*hash)(void*), int (*cmp)(void*, void*));

node_t *create_node(void *k, void *v);

void resize_hashmap(hashmap_t *map);

void *get(hashmap_t *map, void *k);

void *put(hashmap_t *map, void *k, void *v);

void *remove_entry(hashmap_t *map, void *k);

int size(hashmap_t *map);

bool is_empty(hashmap_t *map);

node_t **entries(hashmap_t *map);

void **keys(hashmap_t *map);

void **values(hashmap_t *map);

/**
 * Free all memory associated with the map
 */
void destroy_hashmap(hashmap_t *map);