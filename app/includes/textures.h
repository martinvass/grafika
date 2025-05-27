#ifndef TEXTURES_H
#define TEXTURES_H

#include <GLUT/glut.h>
#include <stdlib.h>

GLuint genSolid(int r,int g,int b);
GLuint genChecker(int w,int h, unsigned char c1[3], unsigned char c2[3]);
GLuint importPainting(const char *filename);

#endif