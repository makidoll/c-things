#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"

typedef enum {false, true} bool;

void processLine(char* line, Obj obj) {
	
	

	printf("%s\n", line);
}

Obj loadObj(char* path) {
	Obj obj;
	obj.totalVertices = 0;
	obj.totalFaces = 0;

	FILE* file;
	file = fopen(path, "r");

	if (!file) {
		fprintf(stderr, "Couldn't read %s!\n", path);
		exit(0);
	}

	Vertex verticies[1000]; // all collected verticies
	//Face faces[1000]; // all collected faces

	char chr;
	char col = 0;
	char line[64];
	memset(line, 0, sizeof(char)*sizeof(line));

	while ((chr=getc(file)) != EOF) {
		line[col] = chr;

		if (chr == ' ') {
			processLine(line, obj);
			memset(line, 0, sizeof(char)*sizeof(line));
			col = 0; continue;
		}; col++;
	}

	return obj;
}