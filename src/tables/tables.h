// header file for table-based memory pool
// first, create the structures needed for the list

// NOTE: the structures should be adaptable
// Because small devices may not need 32 bits for size
// and used can be 1 bit only (if you can fill the other
// 7 bits of padding space)
#include <stdlib.h>
#include <stdbool.h>

typedef struct table_m {
    struct table_m *next;
    bool used;
    void *ptr;
    size_t size;
} table_m;

typedef struct t_pool {
    void *space;
    size_t size;
    struct table_m *table_keeper;
} t_pool;


t_pool *create_pool(size_t size);
void *t_pool_alloc(t_pool *pool, size_t size);
bool t_pool_dealloc(t_pool *pool, void **ptr);
bool destroy_t_pool(t_pool *pool);
bool clean_t_ptr(t_pool *pool, void *ptr);
bool reorder_t_mem(t_pool *pool);
void show_t_mem(t_pool *pool);
