#include "includes/camera.h"
#include "includes/types.h"

float brightness = 1.0f;
const float brightnessStep = 0.1f;

float camX = 0.0f, camY = 1.6f, camZ = 4.0f;
float yaw = 0.0f, pitch = 0.0f;
int lastX = 0, lastY = 0;
bool movingForward = false, movingBackward = false, movingLeft = false,
     movingRight = false, rotating = false;
float speed = 3.0f;

float ROOM_SIZE = 5.0f;

Box obstacles[] = {
    {-2.5f, -1.5f, 1.5f, 2.5f},
    {1.5f, 2.5f, 1.5f, 2.5f}
};
int obstacleCount = sizeof(obstacles) / sizeof(Box);

bool collides(float x, float z) {
  if (x < -ROOM_SIZE + 0.1f || x > ROOM_SIZE - 0.1f || z < -ROOM_SIZE + 0.1f ||
      z > ROOM_SIZE - 0.1f) {
    return true;
  }

  for (int i = 0; i < obstacleCount; i++) {
    Box b = obstacles[i];
    if (x > b.minX && x < b.maxX && z > b.minZ && z < b.maxZ) return true;
  }
  return false;
}

void updateCamera(float delta) {
  float rad = -yaw * M_PI / 180.0f;
  float forwardX = sinf(rad), forwardZ = -cosf(rad);
  float rightX = cosf(rad), rightZ = sinf(rad);
  float dx_total = 0, dz_total = 0;
  if (movingForward) {
    dx_total += forwardX;
    dz_total += forwardZ;
  }
  if (movingBackward) {
    dx_total -= forwardX;
    dz_total -= forwardZ;
  }
  if (movingRight) {
    dx_total += rightX;
    dz_total += rightZ;
  }
  if (movingLeft) {
    dx_total -= rightX;
    dz_total -= rightZ;
  }

  float dist = sqrtf(dx_total * dx_total + dz_total * dz_total) * speed * delta;
  if (dist < 1e-6f) return;

  const float maxStep = 0.1f;
  int steps = (int)ceilf(dist / maxStep);
  float stepDX = dx_total * speed * delta / steps;
  float stepDZ = dz_total * speed * delta / steps;

  for (int i = 0; i < steps; i++) {
    float newX = camX + stepDX;
    float newZ = camZ + stepDZ;
    if (!collides(newX, newZ)) {
      camX = newX;
      camZ = newZ;
    } else {
      break;
    }
  }
}