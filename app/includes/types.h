#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

typedef struct {
  float x, y, z;
} Vec3;

typedef struct {
  Vec3 *verts;
  int vertCount;
  Vec3 *norms;
  int normCount;
  int *vIndex;
  int vIndexCount;
  int *nIndex;
  int nIndexCount;
  int faceCount;
  Vec3 center;
  float scale;
} Mesh;

typedef struct {
  float minX, maxX, minZ, maxZ;
} Box;

void appendVec3(Vec3 **arr, int *count, float x, float y, float z);
void appendInt(int **arr, int *count, int v);

#endif