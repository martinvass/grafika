#ifndef SCENE_H
#define SCENE_H

#include <GLUT/glut.h>
#include "mesh.h"

extern GLuint wallTex, floorTex, paintingTexture, paintingTexture2, paintingTexture3,
    ceilingTex, wallTex2, modelTex, helpTex;

void drawRoom(float s);
void drawPainting(GLuint tex, float x, float y, float z);
void drawPedestal(float x, float y, float z);
void drawModel(Mesh *m, float x, float y, float z);

#endif