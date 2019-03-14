#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

struct _linked_list_t {
	int frame;
	struct _linked_list_t *before;
	struct _linked_list_t *next;
};

typedef struct _linked_list_t linked_list_t;

linked_list_t *head;
linked_list_t *tail;
linked_list_t *nodes;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int lru_evict() {
	return tail->frame;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	int frame = p->frame >> PAGE_SHIFT;
	linked_list_t *this = nodes + frame;
	if (this->before == NULL) {
		// this node is already head, nothing to be done
		return;
	} else {
		this->before->next = this->next;
		if (this->next != NULL) {
		    this->next->before = this->before;
		} else {
			tail = this->before;
		}
		this->before = NULL;
		this->next = head;
		head->before = this;
		head = this;
	}
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	head = NULL;
	tail = NULL;
	nodes = malloc(sizeof(linked_list_t) * memsize);
	for (int i = 0; i < memsize; i++) {
		nodes[i].frame = i;
		nodes[i].next = NULL;
		nodes[i].before = NULL;
	}
	head = nodes;
	tail = nodes;
	for (int i = 1; i < memsize; i++) {
		linked_list_t *this = nodes+i;
		tail->next = this;
		this->before = tail;
		tail = this;
	}
	return;
}

void lru_destroy() {
	free(nodes);
}
