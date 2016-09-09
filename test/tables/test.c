#include <stdio.h>
#include "../../src/tables/tables.h"

// TODO: add assert.h and write tests
void test_create(size_t) {

}

void test_alloc() {

}

void test_dealloc() {

}

void test_cleanup() {

}

int main() {
    t_pool *pool = create_pool(32);
    show_t_mem(pool);
    int *ary = t_pool_alloc(pool, sizeof(int));
    //printf("ary => %p", ary);
    int *ary1 = t_pool_alloc(pool, sizeof(int));
    int *sum = t_pool_alloc(pool, sizeof(int));
    *ary = 5;
    *ary1 = 25;
    printf("ary => %i\n", *ary);
    printf("ary1 => %i\n", *ary1);
    //int *sum = t_pool_alloc(pool, sizeof(int));
    *sum = *(int *)ary + *(int *)ary1;
    printf("%i + %i => %i\n", *ary, *ary1, *sum);
    int *eq = t_pool_alloc(pool, sizeof(int) * 3);
    *(eq) = *ary;
    *(eq + 1) = *ary1;
    //eq[0] = ary;
    //*eq[1] = *ary1;
    *(eq+2) = *sum;
    printf("eq[0] => %i\n", *eq);
    printf("eq[1] => %i\n", *(eq+1));
    printf("%i + %i = %i\n", eq[0], eq[1], eq[2]);
    show_t_mem(pool);

    // cleanup
    destroy_t_pool(pool);
}
