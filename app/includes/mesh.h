#ifndef MESH_H
#define MESH_H

#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

Mesh* loadOBJ(const char *path);
void unitizeMesh(Mesh *m);
void drawMesh(const Mesh *m, GLuint tex);

#endif