#ifndef OBJ_H
#define OBJ_H

typedef struct { // 3d positional data
	float x; float y; float z;
} Vertex;

typedef struct { // indexes to vertices
	int a; int b; int c;
} Face;

enum { OBJ_V, OBJ_F };

typedef struct {
	Vertex* vertex;
	Face* faces;
} Obj;

Obj loadObj(char* path);

#endif