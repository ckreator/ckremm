// Header file for memory pool
/* Function Descriptions:
 * - h_pool create_pool(size_t size, [size_t sector_size]); ==> allocate pool of specified size
 * - pool_entry pool_alloc(h_pool *pool, void *object, size_t size); ==> insert 'object' to pool and return an identifier struct
 * - bool pool_dealloc(h_pool *pool, pool_entry *id); ==> deallocates object, if it's in 'pool'
 * - bool destroy_pool(h_pool *pool); ==> cleanup the pool
 */

/* Mempool should have:
 * - size
 * - ptr(address of beginning)
 * - id_table(allocate in the beginning)
 * - used (useful to know how much space is still left
 */

#include <stdint.h>
// TODO: Check if C is C99 or newer
#include <stdbool.h>

typedef struct Header {
    unsigned int size; // size of element
    struct Header *next; // points to next free Header
} Header;

typedef struct h_pool {
    size_t size; // size of pool
    size_t used; // get amount of used memory
    Header *ptr; // address of beginning
    Header *freeptr; // address of next free ptr
    //Header *entries; // list of entries
} h_pool;


// function declarations
h_pool *create_pool(size_t size);
// returns pointer to memory section
void *pool_alloc(h_pool *pool, size_t size);
void pool_dealloc(h_pool *pool, Header *ap);
bool destroy_pool(h_pool *pool);
void show_pool(h_pool *pool);
