// Header file for memory pool
/* Function Descriptions:
 * - pool_t create_pool(size_t size, [size_t sector_size]); ==> allocate pool of specified size
 * - pool_id pool_alloc(pool_t *pool, void *object, size_t size); ==> insert 'object' to pool and return an identifier struct
 * - bool pool_dealloc(pool_t *pool, pool_id *id); ==> deallocates object, if it's in 'pool'
 * - bool destroy_pool(pool_t *pool); ==> cleanup the pool
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

typedef struct pool_id {
    void *ptr; // address where it begins
    size_t size; // size of element
    struct pool_id *prev;
    struct pool_id *next;
} pool_id;

typedef struct pool_t {
    size_t size; // size of pool
    size_t used; // get amount of used memory
    size_t block_size; // this is the size for the smallest addressable memory unit
    void *ptr; // address of beginning
    pool_id *id_table; // is in essence just a linked list
    size_t id_table_size;
} pool_t;


// function declarations
pool_t *create_pool(size_t size);
pool_id *pool_alloc(pool_t *pool, void *object, size_t size);
bool *pool_dealloc(pool_t *pool, pool_id *id);
bool destroy_pool(pool_t *pool);
