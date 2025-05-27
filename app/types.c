#include "includes/types.h"

void appendVec3(Vec3 **arr, int *count, float x, float y, float z) {
  *arr = realloc(*arr, sizeof(Vec3) * (*count + 1));
  (*arr)[(*count)++] = (Vec3){x, y, z};
}

void appendInt(int **arr, int *count, int v) {
  *arr = realloc(*arr, sizeof(int) * (*count + 1));
  (*arr)[(*count)++] = v;
}