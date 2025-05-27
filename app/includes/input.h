#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GLUT/glut.h>

bool intersectRayWithPlaneZ(int mouseX, int mouseY, float planeZ,
                            float *outX, float *outY);
void keyDown(unsigned char key,int x,int y);
void keyUp(unsigned char key,int x,int y);
void specialKey(int key,int x,int y);
void mouseButton(int button,int state,int x,int y);
void mouseMotion(int x,int y);

#endif