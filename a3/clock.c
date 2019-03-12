#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int clockhand;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	while(coremap[clockhand].ref != 0){
		coremap[clockhand].ref = 0;
		clockhand++;
		if(clockhand == memsize){
			clockhand = 0;
		}
	}
	int value = clockhand;
	clockhand++;
	if(clockhand == memsize){
		clockhand = 0;
	}
	return value;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	coremap[p->frame >> PAGE_SHIFT].ref = 1;
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	clockhand = 0;
	for(int i=0; i< memsize; i++){
		coremap[i].ref = 0;
	}
}
