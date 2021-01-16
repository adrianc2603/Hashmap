#include <stdio.h>
#include <string.h>
#include "main.h"

/**
 * Hash function for each name
 */
size_t hash(char *name) {
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
int cmp(char *name1, char *name2) {
    return strcmp(name1, name2);
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
            printf("%s - ", tmp->key);
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
    if (all_entries == NULL) {
        printf("Entries is NULL because there are no entries\n"); // This should print
    }
    else {
        printf("Entries isn't NULL, but it should be");
        free(all_entries);
    }

    void **all_keys = keys(map);
    if (all_keys == NULL) {
        printf("Keys is NULL because there are no entries\n"); // This should print
    }
    else {
        printf("Keys isn't NULL, but it should be");
        free(all_keys);
    }

    void **all_values = values(map);
    if (all_values == NULL) {
        printf("Values is NULL because there are not entries\n"); // This should print
    }
    else {
        printf("Values isn't NULL, but it should be");
        free(all_values);
    }
}   

/**
 * Helper function to check all_entries, all_keys and all_values are not NULL
 */
void check_all_entries_keys_values(hashmap_t *map) {
    node_t **all_entries = entries(map);
    if (all_entries != NULL) {
        printf("Entries is not NULL because there are entries in the map\n"); // This should print
        free(all_entries);
    }
    else {
        printf("Entries is NULL, but it shouldn't be");
    }

    void **all_keys = keys(map);
    if (all_keys!= NULL) {
        printf("Keys is not NULL because there are entries in the map\n"); // This should print
        free(all_keys);
    }
    else {
        printf("Keys is NULL, but it shouldn't be");
    }

    void **all_values = values(map);
    if (all_values != NULL) {
        printf("Values is not NULL because there are entries in the map\n"); // This should print
        free(all_values);
    }
    else {
        printf("Values is NULL, but it shouldn't be");
    }
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
    printf("The size of the map is %d\n", size(map)); // The size of the map is 0
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 1 (True)
    check_all_entries_keys_values_null(map);

    // Add Adrian 
    char *a_name = create_name("Adrian");
    int *a_age = create_age(20);
    put(map, a_name, a_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 1
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);
    
    // Add Eliza 
    char *b_name = create_name("Eliza");
    int *b_age = create_age(19);
    put(map, b_name, b_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 2
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Add Michael
    char *c_name = create_name("Michael");
    int *c_age = create_age(20);
    put(map, c_name, c_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 3
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Replace Eliza
    printf("Replacing Eliza...\n");
    char *d_name = create_name("Eliza");
    int *d_age = create_age(20);
    int *old_eliza_age = put(map, d_name, d_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 3
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);
    printf("Old Eliza's age is %d\n", *(int*)(old_eliza_age)); // Old Eliza's age is 19
    printf("New Eliza's age is %d\n", *(int*)(get(map, d_name))); // New Eliza's age is 20

    // Add
    char *e_name = create_name("Adam");
    int *e_age = create_age(27);
    put(map, e_name, e_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 4
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Add John
    printf("Adding John now ... should resize\n");
    char *f_name = create_name("John");
    int *f_age = create_age(27);
    put(map, f_name, f_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 5
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Replace John
    printf("Replacing John\n");
    char *g_name = create_name("John");
    int *g_age = create_age(28);
    int *old_john_age = put(map, g_name, g_age);
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 5
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);
    printf("Old John's age is %d\n", *(int*)(old_john_age)); // Old John's age is 27
    printf("New John's age is %d\n", *(int*)(get(map, g_name))); // New John's age is 28

    // Check ages
    printf("Adrian's age is %d\n", *(int*)(get(map, a_name))); // Adrian's age is 20
    printf("Michael's age is %d\n", *(int*)(get(map, c_name))); // Michael's age is 20
    printf("Eliza's age is %d\n", *(int*)(get(map, d_name))); // Eliza's age is 20
    printf("Adams age is %d\n", *(int*)(get(map, e_name))); // Adams age is 27

    // Remove Michael
    printf("Removing Michael -- ");
    printf("The removed age was %d\n", *(int*)(remove_entry(map, c_name))); // The removed age was 20
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 4
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Remove Adrian
    printf("Removing Adrian -- ");
    printf("The removed age was %d\n", *(int*)(remove_entry(map, a_name))); // The removed age was 20
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 3
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Remove John
    printf("Removing John -- ");
    printf("The removed age was %d\n", *(int*)(remove_entry(map, g_name))); // The removed age was 28
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 2
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Try to Remove John, will fail as he isn't in map
    printf("Trying to remove John again -- ");
    printf("The removed age was %p because John isn't in the map\n", (remove_entry(map, g_name))); // The removed age was 0x0 (NULL) because John isn't in the map
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 2
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)

    // Remove Adam
    printf("Removing Adam -- ");
    printf("The removed age was %d\n", *(int*)(remove_entry(map, e_name))); // The removed age was 27
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 1
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 0 (False)
    check_all_entries_keys_values(map);

    // Remove Eliza
    printf("Removing Eliza -- ");
    printf("The removed age was %d\n", *(int*)(remove_entry(map, d_name))); // The removed age was 20
    print_map(map);
    printf("The size of the map is %d\n", size(map)); // The size of the map is 0
    printf("Is the map empty? %d\n", is_empty(map)); // Is the map empty? 1 (True)
    check_all_entries_keys_values_null(map);

    // Add these to ensure destroy_hashmap frees nodes in map and map itself
    printf("Adding Adrian, Michael and Eliza back\n");
    put(map, a_name, a_age);
    put(map, c_name, c_age);
    put(map, d_name, d_age);
    print_map(map);

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