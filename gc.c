
#include <stdlib.h>
#include <stdio.h>
#include "gc.h"

int *getBlock(int *p);
void mark(int *p);
void sweep();


// given function that returns bottom memory address of stack
void *stackBottom() {
    unsigned long bottom;
    FILE *statfp = fopen("/proc/self/stat", "r");
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*u %*u %*u %*u %*u %*u %*d %*d "
           "%*d %*d %*d %*d %*u %*u %*d "
           "%*u %*u %*u %lu", &bottom);
    fclose(statfp);
    return (void *) bottom;
}


void gc() {
    int ** max = (int**) stackBottom();
    int *dummy;
    int **p = &dummy;
    
    while (p < max) {
        mark(*p);
        p++;
    }
    
    sweep();
    coalesce();
}


// mark non-garbage blocks
void mark(int *p) {
    // find block
    // see if p points into the heap
    // if it does, find the block it is in and mark it
    
    int *firstblock = (int*)firstBlock();
    int *lastblock = (int*)lastBlock();
    
    int *ptr = (int*) p;
    if (ptr < firstblock || ptr > lastblock) {
        return;
    }
    
    int *b = getBlock(ptr);
    if (isAllocated(b)) {
        *b = *b + 2;
        int bsize = *b/4;
        int **p2 = (int**)b+1;
        
        while (p2 <= (int**)(b + bsize)) {
            mark(*p2);
            p2 = p2 + 1;
        }
    }
}

// free non-marked blocks
void sweep() {
    int *p = (int*)firstBlock();
    int *end = (int*)lastBlock();
    
    while (p != end) {
        if (*p%4 != 3) {
            myfree(p+1);
        } else {
            *p = *p-2;
        }
        p = (int*) nextBlock(p);
    }
    coalesce();
}

// return the header of input block
int *getBlock(int *p) {
    int *block = (int*)firstBlock();
    
    while ((int*)nextBlock(block) < p) {
        block = (int*)nextBlock(block);
    }
    
    return block;
}
