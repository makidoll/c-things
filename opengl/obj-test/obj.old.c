// This algorithm wasn't very well thought out.
// It's optimized, however, it's super messy to work in
// and I can't get my head over it. Instead, I'm rewriting
// this but I'm gonna use more functions and strings.

#include <stdio.h>
#include <stdlib.h>

#include "obj.h"

typedef enum {false, true} bool;

ObjType checkObject(char* word) {
	if (
		word[0] == 'v' &&
		word[1] == ' ') return OBJ_V;

	if (
		word[0] == 'f' &&
		word[1] == ' ') return OBJ_F;

	return OBJ_NONE;
}

Obj loadObj(char* path) {
	Obj obj;

	FILE* file;
	file = fopen(path, "r");

	if (!file) {
		fprintf(stderr, "Couldn't read %s!\n", path);
		exit(0);
	}

	Vertex verticies[1000]; // all collected verticies
	//Face faces[1000]; // all collected faces

	char chr; // current character
	int col = 0; // current column in word
	int line = 0; // current link

	char word[32]; // used for storing numbers and objType
	ObjType type = -1; // current lines objType

	bool skipObjTypeCheck = false; // if objType has been read

	int currentVertex = 0; // starts at 0!
	int currentVertexDimension = 0; // goes from 0 to 2 because 3d

	while ((chr=getc(file)) != EOF) {
		//printf("%d %d %d\n", word[0], word[1], word[2]);

		if (chr == 10) { // new line
			// check to convert string to number
			switch (type) {
				case OBJ_V: 
					verticies[currentVertex].position[currentVertexDimension] = atof(word);
					currentVertex++;
					break;
			}

			// reset to new line
			line++; 
			col = -1;
			skipObjTypeCheck = false;
			continue;
		}; col++;

		word[col] = chr;
		if (chr == ' ') {
			col = -1; // reset column back 

			if (!skipObjTypeCheck) {
				type = checkObject(word);
				skipObjTypeCheck = true;
				continue;
			} // objType has been found

			if (type == OBJ_V) {
				verticies[currentVertex].position[currentVertexDimension] = atof(word);
				currentVertexDimension++;
			}

			continue; 
		}		
	}

	printf("%2.4f %2.4f %2.4f\n", 
		verticies[2].position[0],
		verticies[2].position[1],
		verticies[2].position[2]
	);

	return obj;
}