#include <stdio.h>
#include <stdlib.h>

#include "obj.h"

#define MAX_LINE 1000

Obj loadObj(char* path) {
	Obj obj;

	FILE* file;
	file = fopen(path, "r");

	char chr;
	int col = -1;
	int line = 0;

	// used for reading inbetween spaces
	char[MAX_LINE] word;
	word[0] = EOF;

	char type = -1;

	while ((chr=getc(file)) != EOF) {
		if (chr == 10) { // new line
			line++; 
			col = -1;
			type = -1; // reset type indicator
			continue;
		}; col++;

		if (type < 0) {
			if (
				chr == 35  || // #: comment
				chr == 118    // v: vertex
			) {
				type = chr;
				printf("Setting obj type to: %d\n", chr);
			}
			continue;
		}

		switch(type) {
			case 35: // #: comment
				continue;
				break;
			case 118: // v: vertex
				printf("\tReading vertice from %d\n", line);
				break;
		}
	}

	if (!file) {
		fprintf(stderr, "Couldn't read %s!\n", path);
		exit(0);
	}

	return obj;
}