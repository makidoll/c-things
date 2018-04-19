#ifndef OBJ_H
#define OBJ_H

typedef struct {
	float vertices[1000][3];
	int faces[1000][3];

	int totalVertices;
	int totalFaces;
} Obj;

Obj loadObj(char* path);

#endif