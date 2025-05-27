#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include <stdbool.h>
#include <GLUT/glut.h>

extern Mesh *model1, *model2;

extern int selectedPainting;
extern bool showHelp;

void display();
void reshape(int w, int h);

#endif