#ifndef OBJ_H
#define OBJ_H

typedef struct { // 3d positional data
	float position[3];
} Vertex;

typedef struct { // indexes to vertices
	int index[3]; // triangluated only
} Face;

typedef enum { 
	OBJ_NONE,
	OBJ_V,
	OBJ_F
} ObjType;

typedef struct {
	Vertex* vertex;
	Face* faces;

	int totalVertices;
	int totalFaces;
} Obj;

Obj loadObj(char* path);

#endif