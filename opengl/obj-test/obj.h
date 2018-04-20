#ifndef OBJ_H
#define OBJ_H

typedef struct {
	float vertices[1000][3];
	int faces[1000][3];

	int totalVertices;
	int totalFaces;
} Obj;

int loadObj(Obj* obj, char* path);

#endif