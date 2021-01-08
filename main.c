#include "main.h"
 
/**
 * Initialise hashmap and all fields. Return a pointer to the map
 */
hashmap_t *create_hashmap(size_t (*hash)(void*), int (*cmp)(void*, void*)) {
    hashmap_t *map = malloc(sizeof(hashmap_t));
    map->num_entries = 0;
    map->capacity = 32; // Start with a capacity of 32. 
    map->entries = malloc(sizeof(node_t*) * map->capacity);

    map->hash = hash;
    map->cmp = cmp;

    // Initalise entries as NULL 
	for (int i = 0; i < map->capacity; i++) {
		map->entries[i] = NULL;
	}
    return map;                        
}

/**
 * Create a new node with the given key and value. Return a pointer to it
 */
node_t *create_node(void *k, void *v) {
    node_t *new_node = malloc(sizeof(node_t)); 
    new_node->key = k;
    new_node->value = v;
    new_node->next = NULL;
    return new_node;
}

/**
 * Double the size of the hashmap
 */
void resize_hashmap(hashmap_t *map) {

	// Create new entries 
	int new_size = map->capacity * 2;
	node_t **new_entries = malloc(sizeof(node_t*) * new_size);

	// Add everything from map->entries into new_entries 
	for (int i = 0; i < map->capacity; i++) {
		node_t *current_node = map->entries[i];
		while (current_node != NULL) {

            // Get index in new hashmap
			size_t index = map->hash(current_node->key) % new_size;

            // No entry exists at the index
			if (new_entries[index] == NULL) {
				new_entries[index] = create_node(current_node->key, current_node->value);
			}	

            // Add new node to the end of the separate chain
			else {
                node_t *last_node = new_entries[index];
				while (last_node->next != NULL) {
                    last_node = last_node->next;
				}
                last_node->next = create_node(current_node->key, current_node->value);
			}

			current_node = current_node->next;
		}
	}

	// Remove everything from map entries
	for (int i = 0; i < map->capacity; i++) {
		node_t *current_node = map->entries[i];
		node_t *next_node = NULL;

		while (current_node != NULL) {
			next_node = current_node->next;
			free(current_node);
			current_node = next_node;
		}
	}

	// Replace map->entries with new_entries
	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_size;
}

/**
 * If the map has a node with key k, return its assosciated value.
 * Return NULL otherwise
 */
void *get(hashmap_t *map, void *k) {
    if (map == NULL || k == NULL) {
        return NULL;
    }
    size_t index = map->hash(k) % map->capacity;

    node_t *current_node = map->entries[index];

	while (current_node != NULL && map->cmp(current_node->key, k) != 0) {
		current_node = current_node->next;
	}

	// Return NULL if key does not exist
	if (current_node == NULL) {
		return NULL;
	}

	return current_node->value;
}

/**
 * Insert node (k, v) into the map M. If the map has an entry with key k, 
 * remove it and return its associated value. Return NULL otherwise
 * Note: Collisions are dealt with using Separate Chaining
 */
void *put(hashmap_t *map, void *k, void *v) {
	if (map == NULL || k == NULL || v == NULL) {
		return NULL;
	}

    // Resize hashmap if num_elements is 66% of the capacity
	float percentage_full = (float) map->num_entries / map->capacity * 100;
	if (percentage_full >= 66) {
		resize_hashmap(map);
	}

	size_t index = map->hash(k) % map->capacity;

	// Remove node with key k if there is one
	void *ret = remove_entry(map, k);

	// If node is not already in the map
	if (map->entries[index] == NULL) {
		map->entries[index] = create_node(k, v);
	}

    // Add to chain of bucket in map (Separate Chaining)
	else {
		node_t *current_node = map->entries[index];
		while (current_node->next != NULL) {
			current_node = current_node->next;
		}
		current_node->next = create_node(k, v);
	}

	map->num_entries++;
    return ret;
}

/**
 * If the map has a node with key k, remove it return its 
 * associated value. Otherwise, return NULL
 */
void *remove_entry(hashmap_t *map, void *k) {
	if (map == NULL || k == NULL) {
		return NULL; 
	}

    void *removed_value = NULL;

	size_t index = map->hash(k) % map->capacity;

	node_t *current_node = map->entries[index];
	node_t *prev_node = NULL;

	while (current_node != NULL && map->cmp(current_node->key, k) != 1) {
		prev_node = current_node;
		current_node = current_node->next;
	}

	// Node with key k wasn't found
	if (current_node == NULL) {
		return NULL;
	}

	// Node with key k was head of the separate chain
	if (prev_node == NULL) {
        removed_value = map->entries[index]->value;
		free(map->entries[index]);
		map->entries[index] = current_node->next;
	}	
													
	// Match was not head of the separate chain
	else {
        removed_value = prev_node->next->value;
		free(prev_node->next);
		prev_node->next = current_node->next;
	}

	map->num_entries--;
    return removed_value;
}

/**
 * Return the number of elements stored 
 */
int size(hashmap_t *map) {
    if (map == NULL) {
        return 0;
    }
    return (map->num_entries);
}

/**
 * Return whether the map stored elements or not
 */
bool is_empty(hashmap_t *map) {
    if (map == NULL) {
        return 0;
    }
    return (map->num_entries == 0);
}

/**
 * Return an iterable collection of the entries in the map
 * Time Complexity: O(n) average, O(n^2) worst case
 */
node_t **entries(hashmap_t *map) {
    if (map == NULL) {
        return NULL;
    }

    node_t **all_entries = malloc(sizeof(node_t*) * map->num_entries);
    int insert_index = 0;
    node_t *current_node = NULL;

    // Add each entry into the new array, including the seperately chained ones
    for (int i = 0; i < map->capacity; i++) {
        current_node = map->entries[i];
        while (current_node != NULL) {
            all_entries[insert_index] = current_node; 
            insert_index++;
            current_node = current_node->next;
        }
    }

    return all_entries;
}

/**
 * Return an iterable collection of the keys in the map
 * Time Complexity: O(n) average, O(n^2) worst case
 */
void **keys(hashmap_t *map) {
    if (map == NULL) {
        return NULL;
    }

    void **all_keys = malloc(sizeof(node_t*) * map->num_entries);
    int insert_index = 0;
    node_t *current_node = NULL;

    // Add each key into the new array, including the seperately chained ones
    for (int i = 0; i < map->capacity; i++) {
        current_node = map->entries[i];
        while (current_node != NULL) {
            all_keys[insert_index] = current_node->key; 
            insert_index++;
            current_node = current_node->next;
        }
    }

    return all_keys;
}

/**
 * Return an iterable collection of the values in the map
 * Time Complexity: O(n) average, O(n^2) worst case
 */
void **values(hashmap_t *map) {
    if (map == NULL) {
        return NULL;
    }

    void **all_values = malloc(sizeof(node_t*) * map->num_entries);
    int insert_index = 0;
    node_t *current_node = NULL;

    // Add each value into the new array, including the seperately chained ones
    for (int i = 0; i < map->capacity; i++) {
        current_node = map->entries[i];
        while (current_node != NULL) {
            all_values[insert_index] = current_node->value; 
            insert_index++;
            current_node = current_node->next;
        }
    }

    return all_values;
}

/**
 * Free all memory associated with the map
 */
void destroy_hashmap(hashmap_t *map) {
	if (map == NULL) {
		return;
	}

    // Free all entries, including the separately chained ones
	for (int i = 0; i < map->capacity; i++) {
		node_t *current_node = map->entries[i];
		node_t *next_node = NULL;

        // Separately chained nodes
		while (current_node != NULL) {
			next_node = current_node->next;
			free(current_node);
			current_node = next_node;
		}
	}

	free(map->entries);
	free(map);	
}