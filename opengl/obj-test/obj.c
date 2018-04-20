#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"

typedef enum {false, true} bool;
<<<<<<< HEAD

char const* spaceSep = " ";
char const* slashSep = "/";

void printVertex(float* vertex) {	
	printf("v: %2.2f, %2.2f, %2.2f\n", 
		vertex[0], vertex[1], vertex[2]);
}

void printFace(int* face) {	
	printf("f: %d, %d, %d\n", 
		face[0], face[1], face[2]);
}

int parseFaceIndex(char* data) {
	char* tokenPtr;
	char* token = strtok_r(data, slashSep, &tokenPtr);

	return atoi(token)-1;
};

void parseLine(char* line, Obj* obj) {
	char* tokenPtr;
	char* token = strtok_r(line, spaceSep, &tokenPtr);

	if (strcmp(token, "v")==0) { // vector
		obj->vertices[obj->totalVertices][0] = atof(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->vertices[obj->totalVertices][1] = atof(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->vertices[obj->totalVertices][2] = atof(strtok_r(NULL, spaceSep, &tokenPtr));

		//printVertex(obj->vertices[obj->totalVertices]);
		obj->totalVertices++;
		return;
	}

	if (strcmp(token, "f")==0) { // face
		obj->faces[obj->totalFaces][0] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->faces[obj->totalFaces][1] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->faces[obj->totalFaces][2] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));

		//printFace(obj->faces[obj->totalFaces]);
		obj->totalFaces++;
		return;
	}
}

int loadObj(Obj* obj, char* path) {
	obj->totalVertices = 0;
	obj->totalFaces = 0;
=======

char const* spaceSep = " ";
char const* slashSep = "/";

void printVertex(float* vertex) {	
	printf("v: %2.2f, %2.2f, %2.2f\n", 
		vertex[0], vertex[1], vertex[2]);
}

void printFace(int* face) {	
	printf("f: %d, %d, %d\n", 
		face[0], face[1], face[2]);
}

int parseFaceIndex(char* data) {
	char* tokenPtr;
	char* token = strtok_r(data, slashSep, &tokenPtr);

	return atoi(token)-1;
};

void parseLine(char* line, Obj* obj) {
	char* tokenPtr;
	char* token = strtok_r(line, spaceSep, &tokenPtr);

	if (strcmp(token, "v")==0) { // vector
		obj->vertices[obj->totalVertices][0] = atof(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->vertices[obj->totalVertices][1] = atof(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->vertices[obj->totalVertices][2] = atof(strtok_r(NULL, spaceSep, &tokenPtr));

		printVertex(obj->vertices[obj->totalVertices]);
		obj->totalVertices++;
		return;
	}

	if (strcmp(token, "f")==0) { // face
		obj->faces[obj->totalFaces][0] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->faces[obj->totalFaces][1] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));
		obj->faces[obj->totalFaces][2] = parseFaceIndex(strtok_r(NULL, spaceSep, &tokenPtr));

		printFace(obj->faces[obj->totalFaces]);
		obj->totalFaces++;
		return;
	}
}

Obj loadObj(char* path) {
	Obj obj;
	obj.totalVertices = 0;
	obj.totalFaces = 0;
>>>>>>> refs/remotes/origin/master

	FILE* file;
	file = fopen(path, "r");

	if (!file) {
<<<<<<< HEAD
		//fprintf(stderr, "Couldn't read %s!\n", path);
		return -1;
=======
		fprintf(stderr, "Couldn't read %s!\n", path);
		return obj;
>>>>>>> refs/remotes/origin/master
	}

	char chr;
	char col = 0;
	char line[64];

	while ((chr=getc(file)) != EOF) {
		if (chr == '\n') {
			line[col] = 0;
<<<<<<< HEAD
			parseLine(line, obj);
=======
			parseLine(line, &obj);
>>>>>>> refs/remotes/origin/master
			col = 0;
			continue;
		};

		line[col] = chr;
		col++;
	}

<<<<<<< HEAD
	printf("Loaded: %s\n  %d vertices\n  %d faces\n",
		path, obj->totalVertices, obj->totalFaces);

	return 1;
=======
	printf("Loaded: %s!\n  %d vertices\n  %d faces\n",
		path, obj.totalVertices, obj.totalFaces);

	return obj;
>>>>>>> refs/remotes/origin/master
}