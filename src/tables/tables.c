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

bool t_pool_dealloc(t_pool *pool, void **p) {
    // Steps:
    // DEBUG:
    void *ptr = *p;
    printf("ptr -> %p\n", ptr);
    //return false;
    // 1. Sanity-check pool and ptr
    if (pool == NULL || pool->table_keeper == NULL) {
        printf("pool is invalid\n");
        return false;
    }

    if (ptr == NULL) {
        printf("ptr is invalid\n");
        return false;
    }
    // 2. Search a chunk where ptr belongs
    table_m *search = pool->table_keeper;
    table_m *prev = NULL;
    for (; search != NULL; prev = search, search = search->next) {
        if (ptr >= search->ptr && ptr < (search->ptr + search->size)) {
            break;
        }
    }

    //printf("DEALLOC: search: ptr: %p | size: %lu | used: %s\n", search, search->size, search->used ? "true" : "false");
    //return false;
    if (search == NULL) {
        printf("couldn't find ptr in pool\n");
        return false;
    }

    /*if (!search->used) {
        printf("");
    }*/

    // 3. Check if neighbors are free
    // To avoid possible 'overbranching', we could use a little 'hack'
    // we can use the UNIX method to checkout the permission and apply
    // it here to find whether we need to do:
    // 0 - Don't merge, just release
    // 1 - merge to lower neighbor
    // 2 - merge to upper neighbor
    // 3 - merge to lower and upper neighbor
    // this way we'll have only a simple if-else if-else if statement
    unsigned char mask = 0;
    mask += ((prev != NULL && !prev->used) ? 1 : 0);
    mask += ((search->next != NULL && !search->next->used) ? 2 : 0);
    // now mask is either 0, 1, 2 or 3
    if (mask == 0) {
        search->used = false;
    } else if (mask == 1) {
        // merge to lower
        // TODO: it's probably a good idea to call reorder_t_mem at some point
        prev->next = search->next;
        prev->size += search->size;
        prev->used = false; // just to be sure
        free(search);
    } else if (mask == 2) {
        // merge to upper
        search->used = false;
        search->size += search->next->size;
        // utilize 'prev' pointer
        prev = search->next;
        search->next = search->next->next;
        free(prev); // NOTE: don't be confused by this, I just reused prev
    } else if (mask == 3) {
        // merge both
        prev->used = false;
        prev->next = search->next->next;
        prev->size += search->size + search->next->size;
        // free table entries
        free(search->next);
        free(search);
    }

    return true;
}

bool destroy_t_pool(t_pool *pool) {
    if (pool == NULL)
        return true;
    printf("destroying pool\n");
    if (pool->space)
        free(pool->space);

    printf("freed space\n");
    table_m *p = pool->table_keeper;
    table_m *o;
    for (; p != NULL;) {
        printf("destroying table => %p | ptr: %p | size: %lu\n", p, p->ptr, p->size);
        o = p->next;
        free(p);
        p = o;
    }
    printf("freed table\n");
    //free(p);
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
