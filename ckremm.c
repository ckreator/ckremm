#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>
#include "ckremm.h"

#define ID_SECTOR_SIZE 8

pool_t *create_pool(size_t size) {
    if (size) {
        void *ptr = calloc(size, ID_SECTOR_SIZE);
        pool_t *pool = malloc(sizeof(pool_t));
        if (pool != NULL || pool != ptr) {
            pool->ptr = ptr;
            pool->size = size;
            pool->used = 0;
            // TODO: table size should be set by 'sector_size'
            pool->id_table = malloc(sizeof(pool_id) * ID_SECTOR_SIZE);
            pool->id_table_size = ID_SECTOR_SIZE;
            // TODO: create dynamic block sizing
            pool->block_size = sizeof(char);
            return pool;
        }
    }
    return NULL;
}

/* Allocation Steps:
 * - find free id with required size
 * -
 *
 */

pool_id *find_free_id(pool_t *pool, size_t size) {
    // find free id entry

    return NULL;
}

pool_id *pool_alloc(pool_t *pool, void *object, size_t size) {
    pool_id *p_id = find_free_id(pool, size);
    if (p_id != NULL) {
        p_id->size = size;
        return p_id;
    }
    return NULL;
}

bool *pool_dealloc(pool_t *pool, pool_id *id) {
    return false;
}

bool destroy_pool(pool_t *pool) {
    if (pool != NULL) {
        free(pool->ptr);
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    pool_t *p = create_pool(16);
    printf("size of p => %lu\n", p->size);
    destroy_pool(p);
    return 0;
}
