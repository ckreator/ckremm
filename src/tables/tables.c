// source code for table-based memory pool
#include "tables.h"

// TODO: don't use cyclic linked lists, make the last one point to NULL
// That way it'll be easier to iterate through them (or cleaner)

// DEBUG:
#include <stdio.h>

// size means the size in bytes
t_pool *create_pool(size_t size) {
    t_pool *ret = malloc(sizeof(t_pool));

    // don't operate on NULL
    if (ret == NULL)
        return NULL;
    ret->size = size;

    // create table_keeper
    ret->table_keeper = malloc(sizeof(table_m));
    // make sure table_keeper was allocated
    if (ret->table_keeper == NULL) {
        free(ret);
        return NULL;
    }
    ret->table_keeper->next = NULL; //ret->table_keeper;
    ret->table_keeper->size = size;

    // create the 'pool' itself
    ret->space = malloc(sizeof(size));

    // Make sure ret->space was allocated
    if (ret->space == NULL) {
        free(ret);
        return NULL;
    }
    ret->table_keeper->ptr = ret->space;

    // else, just return the structure
    return ret;
}

/* Allocation mechanism:
 * 1. Check the pool and size for proper values
 * 2. Walk through the list of blocks and search for a fit in size
 * 3. If block found, mark it as used, resize if needed and return. If not found, return NULL
 *
 */
void *t_pool_alloc(t_pool *pool, size_t size) {
    // TODO: make sure pool has proper values
    if (pool == NULL || !size)
        return NULL;

    table_m *search = pool->table_keeper;
    if (search == NULL)
        return NULL;

    // 2. Walk through the list
    //bool found = (!pool->table_keeper->used && pool->table_keeper->size >= size);
    //search = search->next;
    //printf("initial FOUND: %s | %s\n", found == true ? "true" : "false", pool->table_keeper->used ? "true" : "false");
    for (; search != NULL; search = search->next) {
        // Let's use 'first-match' in the beginning
        // TODO: add flag to be able to switch
        // between 'first-match' and 'best-match'
        if (search->size >= size && !search->used) {
            //found = true;
            //printf("found SEARCH: %p | size: %lu | used: %s\n", search, search->size, search->used ? "true" : "false");
            break;
        }
    }
    // If not found, return NULL
    //if (!found)
    if (search == NULL)
        return NULL;

    table_m *nu;

    // DEBUG:
    table_m *p = search;
    // resize if needed
    if (search->size > size) {
        // insert list entry
        nu = malloc(sizeof(table_m));
        nu->size = search->size - size;
        nu->ptr = search->ptr + size;
        nu->next = search->next;
        nu->used = false;

        search->next = nu;
        search->size = size;
        // If found, mark it as used
        search->used = true;
        //printf("SEARCH => ptr: %p | size: %lu | used: %s | next: %p\n", p->ptr, p->size, p->used ? "true" : "false", p->next);
    }
    //printf("SEARCH => ptr: %p | size: %lu | used: %s | next: %p\n", search->ptr, search->size, search->used ? "true" : "false", search->next);
    //p = nu;
    //printf("NU => p: %p | ptr: %p | size: %lu | used: %s | next: %p\n", nu, nu->ptr, nu->size, nu->used ? "true" : "false", nu->next);
    return search->ptr;
}

bool t_pool_dealloc(t_pool *pool, void *ptr) {
    return false;
}

bool destroy_t_pool(t_pool *pool) {
    if (pool == NULL)
        return true;
    if (pool->space)
        free(pool->space);
    table_m *p = pool->table_keeper;
    table_m *o;
    for (; p->next != NULL;) {
        o = p->next;
        free(p);
        p = o;
    }
    free(p);
    return false;
}

bool clean_t_ptr(t_pool *pool, void *ptr) {
    return false;
}


bool reorder_t_mem(t_pool *pool) {
    return false;
}


// DEBUG:
void show_t_mem(t_pool *pool) {
    table_m *p = pool->table_keeper;
    for (; p != NULL; p = p->next) {
        printf("p: %p | ptr: %p | size: %lu | used: %s | next: %p\n", p, p->ptr, p->size, p->used ? "true" : "false", p->next);
    }
    puts("================================");
}
