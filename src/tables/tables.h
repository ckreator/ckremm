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
// if you're going to rearrange the pool, the pointers will be invalid
// that's why you need to obtain the chunk table entry itself
table_m *__get_section(t_pool *pool, size_t size);

// helper function to avoid segmentation when deallocating
bool __t_smart_dealloc(table_m *search, table_m *prev);

// Works stable only if you don't plan to rearrange the pool
bool t_pool_dealloc(t_pool *pool, void **ptr);

// Deallocates elements under entry. Unless entry has been
// already deleted, this is stable
bool t_pool_dealloc_s(t_pool *pool, table_m *entry);

bool destroy_t_pool(t_pool *pool);

bool clean_t_ptr(t_pool *pool, void *ptr);

// rearrange memory pool, free up all the unused fragments
bool reorder_t_mem(t_pool *pool);

// copies the data from one entry to another. works only if
// dest.size >= src.size
void *t_copy_mem(table_m *dest, table_m *src);

// write data 'count' times to dest
bool t_write_to(table_m *dest, void *data, size_t count);

// good for debugging, not really part of API
void show_t_mem(t_pool *pool);
