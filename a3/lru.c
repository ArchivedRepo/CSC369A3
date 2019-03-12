#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

struct frame *head;

struct frame *tail;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
	return tail->pte->frame >> PAGE_SHIFT;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	int frame = p->frame >> PAGE_SHIFT;

	// if it is not head
	if(coremap[frame].before != NULL){
		// if the frame is tail
		if(coremap[frame].after == NULL){
			tail = coremap[frame].before;
			tail->after = NULL;
		}else{
			coremap[frame].before->after = coremap[frame].after;
			coremap[frame].after->before = coremap[frame].before;
		}
		
		coremap[frame].before = NULL;
		coremap[frame].after = head;
		head->before = &coremap[frame];
		head = &coremap[frame];
	}
	
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	head = &coremap[0];
	tail = &coremap[memsize - 1];

	coremap[0].before = NULL;
	coremap[0].after = &coremap[1];
	for(int i=1; i < memsize - 1; i++){
		coremap[i].before = &coremap[i-1];
		coremap[i].after = &coremap[i+1];
	}
	coremap[memsize - 1].before = &coremap[memsize - 2];
	coremap[memsize - 1].after = NULL;
}
