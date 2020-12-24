/**
 * Program to load information into a struct from a given file
 * @author Noah O'Brien
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "DataLoader.h"

struct test_scenario* load_test_data(char* filename){

	struct test_scenario* data = malloc(sizeof(struct test_scenario));

	FILE *file = fopen(filename, "r");

	fscanf(file, "%d", &data->page_count);
	fscanf(file, "%d", &data->frame_count);
	fscanf(file, "%d", &data->refstr_len);

	for(int i = 0; i < data->refstr_len; i++){

		fscanf(file, "%d", &data->refstr[i]);

	}

	fclose(file);
	return data;

}
