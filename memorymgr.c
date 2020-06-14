#include <stdio.h>
#include <stdlib.h>
#include "memorymgr.h"



// global variables
int *firstblock;
int *lastblock;
int *firstheader;

int next8(int size);


// initialize a simulated heap
void  initmemory(int size) {
    
    // 39 as original request
    // need 4 bytes for header of heap
    // 4 bytes for sentinal block
    // 4 bytes for alingment
    
    // 39 + 4 + 4 + 4
    int heaplentgh = next8(size + 4);
    
    int *heap = malloc(heaplentgh);
    
    firstblock = heap;
    
    firstheader = heap + 1;
    
    lastblock = firstblock + (heaplentgh/4) - 1;
    
    *firstheader = heaplentgh - 8;
    
    *lastblock = 1;
    
    
}


// allocate memory from the heap
void *myalloc(int length) {
    
    int lastVal = 0;
    int *header = firstBlock();
    int *ptr;
    int size = next8(length);
    
    // Look for unallocated block
    while (*header < size || *header % 2 !=0) {
        
        // we've reached the sentinel block, so no valid block
        if (*header == 1) {
            return NULL;
        }
        header = nextBlock(header);
    }
    
    lastVal = *header;
    // +1 to show that block is allocated
    *header = size + 1;
    // to return the block (not header), we want the header+1
    ptr = header + 1;
    // adjust rest of heap
    header = nextBlock(header);
    if (*header != 1) {
        *header = lastVal - size;
    }
    return ptr;
}


// free a block
void  myfree(void *ptr) {
    
    int *header = ptr - 4;
    if(isAllocated(header)) {
        *header = *header - 1;
    }
}

// coalesce all free blocks
void  coalesce() {
    
    int *header = firstBlock();
    int *nextHeader = nextBlock(header);
    
    while (*nextHeader != 1) {
        if (*header % 2 == 0 && *nextHeader % 2 == 0) {
            *header = *header + *nextHeader;
        }
        else {
            header = nextHeader;
        }
        nextHeader = nextBlock(header);
    }
}




void  printallocation() {
    int *header = firstBlock();
    int num = 0;
    while (*header != 1) {
        if (isAllocated(header)) {
            printf("Block %d: size %d \t allocated \n", num, *header - 1);
        }
        
        else {
            printf("Block %d: size %d \t unallocated \n", num, *header);
        }
        
        header = nextBlock(header);
        
        num++;
    }
    printf("\n\n");
}

// The following functions are needed for HW6.
// You should implement them now, as they will also be useful to help you implement HW5.


int next8(int size) {
    size += 4;
    int num = size % 8;
    if (num != 0) {
        size = size + (8 - num);
    }
    return size;
}


// check if block is allocated
int  isAllocated(int *p) {
    if(*p % 8 == 0) {
        return 0;
    }
    return 1;
}

// go the the next block
int *nextBlock(int *p) {
    int *block = p;
    
    if (*block % 2 != 0) {
        block = block + (*block - 1)/4;
    }
    else {
        block = block + *block/4;
    }
    
    return block;
}


// retun the first header in the heap
int *firstBlock() {
    return firstheader;
}

// return sentinel block
int *lastBlock() {
    
    return lastblock;
}





