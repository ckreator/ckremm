#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>
#include "headers.h"

#define SECTOR_SIZE 8

h_pool *create_pool(size_t size) {
    if (size) {
        unsigned int units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
        Header *mem = (Header *)calloc(units, sizeof(Header));
        h_pool *ret = malloc(sizeof(h_pool));
        printf("allocating %lu bytes with %u units | head: %lu\n", units * sizeof(Header), units, sizeof(Header));
        if (mem == NULL && ret == NULL)
            return NULL;

        ret->size = units;
        ret->used = 0;
        ret->ptr = mem;
        ret->freeptr = mem;
        ret->ptr->size = units;
        ret->ptr->next = ret->ptr;
        return ret;
    }
    return NULL;
}

/* Allocation Steps:
 * - find free entry with required or bigger size
 * - [if not found, check for fragments to merge into a larger fragment]
 * - [clean the section]
 * - return new entry with required size (adjust if needed)
 */


void *pool_alloc(h_pool *pool, size_t size) {
    // find first fragment that can hold 'size' memory
    Header *blk = pool->ptr;
    Header *prev;
    unsigned int units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
    //printf("units => %u\n", units);

    for (prev = pool->ptr; ; prev = blk, blk = blk->next) {
        if (blk->size >= size) {
            printf("size fits: %u | %p\n", blk->size, blk);

            if (blk->size > size) {
                blk->size -= units;
                blk += blk->size;
                blk->size = units;

            } else {
                // if sits neatly
                prev->next = blk->next; // 'unlink'
            }
            pool->freeptr = prev;
            printf("blk head has size: %u\n", blk->size);
            Header *ret = blk + 1;
            printf("blk: %p | ret: %p\n", blk, ret);
            return (void *)ret;
        }
        // TODO: send a message or do something better here
        if (blk == pool->ptr)
            return NULL;
    }
    return NULL;
}

bool destroy_pool(h_pool *pool) {
    if (pool != NULL) {
        free(pool->ptr);
        // TODO: free the linked list

        return true;
    }
    return false;
}

void pool_dealloc(h_pool *pool, Header *ap) {
    Header *bp, *p;
    bp = (Header *)ap;
    bp = bp - 1; // point to block header
    //printf("bp has size: %u\n", bp->size);
    // find free block
    //Header *freep = bp->next;
    //printf("pool->ptr => %p | pool->ptr->next: %p\n", pool->ptr, pool->ptr->next);
    //return;
    for (p = pool->freeptr; !(bp > p && bp < p->next); p = p->next) {
        if (p >= p->next && (bp > p || bp < p->next))
            break; // freed block at start or end of pool
    }

    //printf("after first loop: p: %p -> next: %p -> size: %u | bp: %p -> next: %p -> size: %u\n", p, p->next, p->size, bp, bp->next, bp->size);
    //printf("bp + bp->size = %p\n", bp + bp->size);
    if (bp + bp->size == p->next) { // join to upper nbr
        //printf("join to upper\n");
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }

    //printf("p + p->size = %p\n", p + p->size);
    if (p + p->size == bp) { // join to lower header
        //printf("join to lower\n");
        p->size += bp->size;
        p->next = bp->next;
        //pool->freeptr = p;
    } else {
        p->next = bp;
    }

    pool->freeptr = bp;
    //printf("after dealloc: p: %p -> next: %p -> size: %u | bp: %p -> next: %p -> size: %u\n", p, p->next, p->size, bp, bp->next, bp->size);
}

void show_pool(h_pool *pool) {
    Header *pe = pool->ptr;
    printf("Entry | size: %u | ptr: %p\n", pe->size, pe);
    for (pe = pe->next; pe != pool->ptr; pe = pe->next) {
        printf("Entry | size: %u | ptr: %p\n", pe->size, pe);
    }
    puts("===========================");
}
