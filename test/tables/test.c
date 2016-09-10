#include <stdio.h>
#include "../../src/tables/tables.h"
#include <assert.h>

/********* HELPERS **********/
unsigned int count_blocks(t_pool *pool) {
    table_m *p = pool->table_keeper;
    unsigned int count = 0;
    for (; p != NULL; p = p->next, count++) ;
    return count;
}

void test_create(size_t size) {

}

void test_alloc() {
    puts("# Testing ALLOCATION BASICS...");
    t_pool *pool = create_pool(32);
    assert(pool != NULL);
    int *ary = t_pool_alloc(pool, sizeof(int));
    assert(ary != NULL);
    int *ary1 = t_pool_alloc(pool, sizeof(int));
    assert(ary1 != NULL);
    int *sum = t_pool_alloc(pool, sizeof(int));
    assert(sum != NULL);
    int *eq = t_pool_alloc(pool, sizeof(int) * 3);
    assert(eq != NULL);

    // Make sure there are 4 blocks now
    show_t_mem(pool);
    //printf("count_blocks: should be: %u | %u\n", 4, count_blocks(pool));
    puts("### Testing block count...");
    assert(count_blocks(pool) == 5);

    // Yep, we can use 'eq' just as we would use an array, that's fine
    // Go ahead and read 'Deep C Secrets' and you will understand ;D
    eq[0] = 13;
    eq[1] = 99;
    eq[2] = *eq + *(eq+1);
    //printf("eq+2 => %i | %i\n", *(eq+2), eq[2]);
    puts("### Testing 'eq' addition...");
    assert(eq[2] == 112);

    /**ary = 5;
    *ary1 = 25;
    *sum = *ary + *ary1;*/
    //puts("### Testing 'sum' for right value...");
    //assert(sum[0] == 30);

    // cleanup
    destroy_t_pool(pool);
}

void test_dealloc() {
    puts("# Testing BASIC DEALLOCATION...");
    t_pool *pool = create_pool(32);
    assert(pool != NULL);
    int *ary = t_pool_alloc(pool, sizeof(int));
    assert(ary != NULL);
    int *ary1 = t_pool_alloc(pool, sizeof(int));
    assert(ary1 != NULL);
    int *sum = t_pool_alloc(pool, sizeof(int));
    assert(sum != NULL);

    show_t_mem(pool);

    // Make sure there are 4 blocks now
    puts("### Testing block count...");
    assert(count_blocks(pool) == 4);
    // Make sure deallocs all deliver true
    void *p = ary;
    printf("### Testing deallocation...");
    assert(t_pool_dealloc(pool, &p));
    printf("1...");
    p = ary1;
    assert(t_pool_dealloc(pool, &p));
    printf("2...");
    p = sum;
    assert(t_pool_dealloc(pool, &p));
    printf("3...\n");

    // Make sure there is only 1 block now
    assert(count_blocks(pool) == 1);
    puts("### Testing block count...");

    // cleanup
    destroy_t_pool(pool);
}

void test_cleanup() {

}

void test_reorder_first() {
    t_pool *pool = create_pool(32);
    table_m *s1 = __get_section(pool, sizeof(int));
    int *a = s1->ptr;
    table_m *s2 = __get_section(pool, sizeof(int));
    int *b = s2->ptr;
    table_m *s3 = __get_section(pool, sizeof(int));
    int *c = s3->ptr;
    table_m *s4 = __get_section(pool, sizeof(int));
    int *d = s4->ptr;
    *a = 31;
    *b = 3;
    *c = 1995;
    *d = 8;
    show_t_mem(pool);
    void *freed = a;
    t_pool_dealloc(pool, &freed);
    show_t_mem(pool);
    reorder_t_mem(pool);
    // NOTE: To get the new values, you'd need to 'update' all the variables
    // This feature might come in handy, but still requires some extra work
    // from your side
    b = s2->ptr;
    c = s3->ptr;
    d = s4->ptr;
    printf("b is => %i\n", *b);
    printf("c is => %i\n", *c);
    printf("d is => %i\n", *d);
    show_t_mem(pool);
    destroy_t_pool(pool);
}

void test_reorder_middle() {
    t_pool *pool = create_pool(32);
    table_m *s1 = __get_section(pool, sizeof(int));
    int *a = s1->ptr;
    table_m *s2 = __get_section(pool, sizeof(int));
    int *b = s2->ptr;
    table_m *s3 = __get_section(pool, sizeof(int));
    int *c = s3->ptr;
    table_m *s4 = __get_section(pool, sizeof(int));
    int *d = s4->ptr;
    *a = 31;
    *b = 3;
    *c = 1995;
    *d = 8;
    show_t_mem(pool);
    void *freed = c;
    t_pool_dealloc(pool, &freed);
    show_t_mem(pool);
    reorder_t_mem(pool);
    // NOTE: To get the new values, you'd need to 'update' all the variables
    // This feature might come in handy, but still requires some extra work
    // from your side
    a = s1->ptr;
    b = s2->ptr;
    d = s4->ptr;
    printf("a is => %i\n", *a);
    printf("b is => %i\n", *b);
    printf("d is => %i\n", *d);
    show_t_mem(pool);
    destroy_t_pool(pool);
}

void test_reorder_last() {
    t_pool *pool = create_pool(32);
    table_m *s1 = __get_section(pool, sizeof(int));
    int *a = s1->ptr;
    table_m *s2 = __get_section(pool, sizeof(int));
    int *b = s2->ptr;
    table_m *s3 = __get_section(pool, sizeof(int));
    int *c = s3->ptr;
    table_m *s4 = __get_section(pool, sizeof(int));
    int *d = s4->ptr;
    *a = 31;
    *b = 3;
    *c = 1995;
    *d = 8;
    show_t_mem(pool);
    void *freed = d;
    t_pool_dealloc(pool, &freed);
    show_t_mem(pool);
    reorder_t_mem(pool);
    // NOTE: To get the new values, you'd need to 'update' all the variables
    // This feature might come in handy, but still requires some extra work
    // from your side
    a = s1->ptr;
    b = s2->ptr;
    c = s3->ptr;
    printf("a is => %i\n", *a);
    printf("b is => %i\n", *b);
    printf("c is => %i\n", *c);
    show_t_mem(pool);
    destroy_t_pool(pool);
}

int main() {
    //printf("----------------------------------------\nStarting Test Alloc Routine...\n----------------------------------------\n");
//    test_alloc();
    printf("----------------------------------------\nStarting Test Reorder First Routine...\n----------------------------------------\n");
    test_reorder_first();
    printf("----------------------------------------\nStarting Test Reorder Middle Routine...\n----------------------------------------\n");
    test_reorder_middle();
    printf("----------------------------------------\nStarting Test Reorder Last Routine...\n----------------------------------------\n");
    test_reorder_last();
    //printf("----------------------------------------\nStarting Test Dealloc Routine...\n----------------------------------------\n");
    //test_dealloc();
}
