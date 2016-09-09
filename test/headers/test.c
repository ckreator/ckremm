#include <stdio.h>
#include "../../src/headers/headers.h"

// TODO: add assert.h and write tests
void test_create(size_t size) {

}

void test_alloc() {

}

void test_dealloc() {

}

void test_cleanup() {

}

int main(int argc, char **argv) {
    printf("size of unsigned char => %lu\n", sizeof(unsigned char));
    h_pool *pool = create_pool(32 * sizeof(Header));
    printf("size of pool => %lu\n", pool->size);
    show_pool(pool);
    int x = 15;
    void *pe = pool_alloc(pool, sizeof(int) * 2);
    *(int *)pe = x;
    int *xp = (int *)pe;
    xp++;
    x = 99;
    //*(int *)(pe+1) = x;
    *xp = x;
    printf("MAIN: pe: %p | %i || pe[1]: %p | %i\n", pe, *(int *)pe, xp, *xp);
    //x = 99;
    /**(int *)(pe+1) = x;
    void *other = (void *)pe + 1;
    printf("MAIN: pe: %p | %i\n", other, *(int *)other);
    printf("MAIN: pe: %p | %i\n", other, *(int *)(pe+1));
    */
    show_pool(pool);
    void *pep = pool_alloc(pool, sizeof(int));
    show_pool(pool);
    pool_dealloc(pool, pe);
    //pool_dealloc(pool, pep);
    show_pool(pool);
    /*printf("MAIN: size of pe->size %lu\n", pe->size);
    int num = *(int *)get_obj(p, pe);
    printf("num is => %i\n", num);*/
    destroy_pool(pool);
    return 0;
}
