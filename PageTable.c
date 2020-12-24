/**
* Page replacement algorithms
* @author Noah O'Brien
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "PageTable.h"

struct page_table_entry{

	int page;
	int frame;
	int used;
	unsigned int bit[2];

};

struct page_table{

	int page_count;
	int frame_count;
	enum replacement_algorithm algorithm;
	int verbose;
	struct page_table_entry *entry;
	int faults;
	int holder;

};

int findPage(struct page_table *pt, int page);

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose){

	struct page_table *table = malloc(sizeof(struct page_table));

	table->page_count = page_count;
	table->frame_count = frame_count;
	table->algorithm = algorithm;
	table->verbose = verbose;
	table->faults = 0;
	table->entry = malloc(sizeof(struct page_table_entry) * (table->page_count + 1));

	// Initiates values to dirty bit, valid bit and default frame value
	for(int i = 0; i < table->page_count; i++){

		table->entry[i].page = i;
		table->entry[i].bit[0] = 0;
		table->entry[i].bit[1] = 0;

	}

	return table;

}

void page_table_destroy(struct page_table** pt){

	free(*pt);
	pt = NULL;

}

void page_table_access_page(struct page_table *pt, int page){

	// Checks if any of the frames are free using fault count
	if(pt->faults < pt->frame_count){

		pt->entry[page].frame = pt->faults;
		pt->entry[page].bit[1] = 1;
		pt->entry[page].used++;
		pt->faults++;
		return;

	}

	// FIFO replacement
	if(pt->algorithm == 0){

		if(pt->entry[page].bit[1] == 0){

			pt->entry[page].frame = pt->holder;
			pt->entry[page].bit[1] = 1;
			pt->entry[pt->holder].bit[1] = 0;
			pt->faults++;
			pt->holder++;

		}else{

			return;

		}

		if(pt->holder == pt->frame_count - 1){

			pt->holder = 0;

		}

	}

	// LRU replacement
	if(pt->algorithm == 1){

		if(pt->entry[page].bit[1] == 0){

			pt->entry[page].used++;

			int LRU = findPage(pt, page);

			pt->entry[page].frame = pt->entry[LRU].frame;
			pt->entry[page].bit[1] = 1;
			pt->entry[LRU].bit[1] = 0;
			pt->faults++;

		}else{

			pt->entry[page].used++;
			return;

		}

	}

	// MFU replacement
	if(pt->algorithm == 2){

		if(pt->entry[page].bit[1] == 0){

			pt->entry[page].used++;

			int MFU = findPage(pt, page);

			printf("%d", MFU);

			pt->entry[page].frame = pt->entry[MFU].frame;
			pt->entry[page].bit[1] = 1;
			pt->entry[MFU].bit[1] = 0;
			pt->faults++;

		}else{

			pt->entry[page].used++;
			return;

		}

	}

}

// Algorithm to find least or most used page in the entries
int findPage(struct page_table *pt, int page){

	int answer = 0;

	// LRU
	if(pt->algorithm == 1){

		int smallest = pt->entry[0].used;

		for(int i = 1; i < pt->page_count; i++){

			if(smallest > pt->entry[i].used){

				smallest = pt->entry[i].used;
				answer = i;

			}

		}

		return answer;

	// MFU
	}else{

		int largest;

		for(int i = 1; i < pt->page_count; i++){

			largest = pt->entry[i - 1].used;

			if(pt->entry[i - 1].bit[1] == 0){

				continue;

			}

			if(pt->entry[i].bit[1] == 0){

				i++;

				if(i == pt->page_count){

					break;

				}

			}

			if(largest > pt->entry[i].used){

				largest = pt->entry[i].used;
				answer = i - 1;

			}

		}

		return answer;

	}

}

void page_table_display(struct page_table* pt){

	printf("==== Page Table ====\n");

	switch(pt->algorithm){

	case 0:
		printf("Mode: FIFO\n");
		break;
	case 1:
		printf("Mode: LRU\n");
		break;
	case 2:
		printf("Mode: MFU\n");
		break;
	default:
		printf("Algorithm not found.\n");

	}

	printf("Page Faults: %d\n", pt->faults);

	page_table_display_contents(pt);

}

void page_table_display_contents(struct page_table *pt){

	printf("page frame | dirty valid\n");

	for(int i = 0; i < pt->page_count; i++){

		printf("   %d     %d |     %d     %d\n", i, pt->entry[i].frame, pt->entry[i].bit[0], pt->entry[i].bit[1]);

	}

	printf("\n");

}
