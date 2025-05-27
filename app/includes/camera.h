#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include <math.h>

extern float brightness;
extern const float brightnessStep;
extern float camX, camY, camZ;
extern float yaw, pitch;
extern int lastX, lastY;
extern bool movingForward, movingBackward, movingLeft, movingRight, rotating;
extern float speed;
extern float ROOM_SIZE;

bool collides(float x, float z);
void updateCamera(float delta);

#endif