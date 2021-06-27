#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "main.h"

/**
 * Hash function for each name
 */
size_t hash(void *n) {
    char *name = (char*) n;
    int length = strnlen(name, 256);
    size_t hash_value = 0;
    for (int i = 0; i < length; i++) {
        hash_value += name[i];
        hash_value *= name[i];
    }
    return hash_value;
}

/**
 * Compare function for each name
 */
int cmp(void *name1, void *name2) {
    return strcmp((char*)name1, (char*)name2);
}

/**
 * Helper function to print each item in the hashmap, including 
 * those separately chained
 */
void print_map(hashmap_t *map) {
    puts("");
    for (int i = 0; i < map->capacity; i++) {
        if (map->entries[i] == NULL) {
            printf("\t%i\t---\n", i);
            continue;
        }
        printf("\t%i\t", i);
        node_t *tmp = map->entries[i];
        while (tmp != NULL) {
            printf("%s - ", (char*)tmp->key);
            tmp = tmp->next;
        }
        puts("");
    }
    puts("");
}

/**
 * Helper function to check all_entries, all_keys and all_values are NULL
 */
void check_all_entries_keys_values_null(hashmap_t *map) {
    node_t **all_entries = entries(map);
    assert(all_entries == NULL);

    void **all_keys = keys(map);
    assert(all_keys == NULL);

    void **all_values = values(map);
    assert(all_values == NULL);
}   

/**
 * Helper function to check all_entries, all_keys and all_values are not NULL
 */
void check_all_entries_keys_values(hashmap_t *map) {
    node_t **all_entries = entries(map);
    assert(all_entries != NULL);

    void **all_keys = keys(map);
    assert(all_keys != NULL);

    void **all_values = values(map);
    assert(all_values != NULL);

    free(all_entries);
    free(all_keys);
    free(all_values);
}

/**
 * Helper function to heap allocate a string and return a pointer to it
 */
char *create_name(char *s) {
    char *name = malloc(sizeof(char*) * strlen(s) + 1);
    strcpy(name, s);
    return name;
}

/**
 * Helper function to heap allocate an int and return a pointer to it
 */
int *create_age(int i) {
    int *age = malloc(sizeof(int));
    *age = i;
    return age;
}

void test_all_functions_regular_cases() {
    hashmap_t *map = create_hashmap(hash, cmp, 6);

    print_map(map);
    assert(size(map) == 0);
    assert(is_empty(map) == true);
    check_all_entries_keys_values_null(map);

    // Add Adrian 
    char *a_name = create_name("Adrian");
    int *a_age = create_age(20);
    put(map, a_name, a_age);
    print_map(map);
    assert(size(map) == 1);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);
    
    // Add Eliza 
    char *b_name = create_name("Eliza");
    int *b_age = create_age(19);
    put(map, b_name, b_age);
    print_map(map);
    assert(size(map) == 2);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Add Michael
    char *c_name = create_name("Michael");
    int *c_age = create_age(20);
    put(map, c_name, c_age);
    print_map(map);
    assert(size(map) == 3);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Replace Eliza
    printf("Replacing Eliza...\n");
    char *d_name = create_name("Eliza");
    int *d_age = create_age(20);
    int *old_eliza_age = put(map, d_name, d_age);
    print_map(map);
    assert(size(map) == 3);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);
    assert(*(int*)(old_eliza_age) == 19);
    assert(*(int*)(get(map, d_name)) == 20);

    // Add
    char *e_name = create_name("Adam");
    int *e_age = create_age(27);
    put(map, e_name, e_age);
    print_map(map);
    assert(size(map) == 4);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Add John
    printf("Adding John now ... should resize\n");
    char *f_name = create_name("John");
    int *f_age = create_age(27);
    put(map, f_name, f_age);
    print_map(map);
    assert(size(map) == 5);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Replace John
    printf("Replacing John\n");
    char *g_name = create_name("John");
    int *g_age = create_age(28);
    int *old_john_age = put(map, g_name, g_age);
    print_map(map);
    assert(size(map) == 5);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);
    assert(*(int*)(old_john_age) == 27);
    assert(*(int*)(get(map, g_name)) == 28);

    // Check ages
    assert(*(int*)(get(map, a_name)) == 20);
    assert(*(int*)(get(map, c_name)) == 20);
    assert(*(int*)(get(map, d_name)) == 20);
    assert(*(int*)(get(map, e_name)) == 27);

    // Remove Michael
    printf("Removing Michael -- ");
    assert(*(int*)(remove_entry(map, c_name)) == 20);
    print_map(map);
    assert(size(map) == 4);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Remove Adrian
    printf("Removing Adrian -- ");
    assert(*(int*)(remove_entry(map, a_name)) == 20);
    print_map(map);
    assert(size(map) == 3);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Remove John
    printf("Removing John -- ");
    assert(*(int*)(remove_entry(map, g_name)) == 28);
    print_map(map);
    assert(size(map) == 2);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Try to Remove John, will fail as he isn't in map
    printf("Trying to remove John again -- ");
    assert(remove_entry(map, g_name) == NULL);
    print_map(map);
    assert(size(map) == 2);
    assert(is_empty(map) == false);

    // Remove Adam
    printf("Removing Adam -- ");
    assert(*(int*)(remove_entry(map, e_name)) == 27);
    print_map(map);
    assert(size(map) == 1);
    assert(is_empty(map) == false);
    check_all_entries_keys_values(map);

    // Remove Eliza
    printf("Removing Eliza -- ");
    assert(*(int*)(remove_entry(map, d_name)) == 20);
    print_map(map);
    assert(size(map) == 0);
    assert(is_empty(map) == true);
    check_all_entries_keys_values_null(map);

    // Add these to ensure destroy_hashmap frees nodes in map and map itself
    printf("Adding Adrian, Michael and Eliza back\n");
    put(map, a_name, a_age);
    put(map, c_name, c_age);
    put(map, d_name, d_age);
    print_map(map);
    assert(size(map) == 3);
    assert(is_empty(map) == false);

    free(a_name);
    free(a_age);
    free(b_name);
    free(b_age);
    free(c_name);
    free(c_age);
    free(d_name);
    free(d_age);
    free(e_name);
    free(e_age);
    free(f_name);
    free(f_age);
    free(g_name);
    free(g_age);
    destroy_hashmap(map);
}

int main() {

    test_all_functions_regular_cases(); 

    return 0;
}