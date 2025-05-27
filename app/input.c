#include "includes/input.h"
#include "includes/camera.h"
#include "includes/render.h"

GLuint helpTex;

bool intersectRayWithPlaneZ(int mouseX, int mouseY, float planeZ, float *outX,
                            float *outY) {
  int vp[4];
  double proj[16], model[16];
  glGetIntegerv(GL_VIEWPORT, vp);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);

  double wx1, wy1, wz1, wx2, wy2, wz2;
  gluUnProject(mouseX, vp[3] - mouseY, 0.0, model, proj, vp, &wx1, &wy1, &wz1);
  gluUnProject(mouseX, vp[3] - mouseY, 1.0, model, proj, vp, &wx2, &wy2, &wz2);

  double dz = wz2 - wz1;
  if (fabs(dz) < 1e-6) return false;
  double t = (planeZ - wz1) / dz;
  if (t < 0.0 || t > 1.0) return false;

  *outX = wx1 + t * (wx2 - wx1);
  *outY = wy1 + t * (wy2 - wy1);
  return true;
}

void keyDown(unsigned char key, int x, int y) {
  if (key == 'w') movingForward = true;
  if (key == 's') movingBackward = true;
  if (key == 'a') movingLeft = true;
  if (key == 'd') movingRight = true;
  if (key == '+') {
    brightness += brightnessStep;
    if (brightness > 1.0f) brightness = 1.0f;
    glutPostRedisplay();
  }
  if (key == '-') {
    brightness -= brightnessStep;
    if (brightness < 0.0f) brightness = 0.0f;
    glutPostRedisplay();
  }
  if (key == 27) {
    if (selectedPainting >= 0) {
      selectedPainting = -1;
      glutPostRedisplay();
      return;
    }
    exit(0);
  }
  if (key == 27) exit(0);
}

void keyUp(unsigned char key, int x, int y) {
  if (key == 'w') movingForward = false;
  if (key == 's') movingBackward = false;
  if (key == 'a') movingLeft = false;
  if (key == 'd') movingRight = false;
}

void specialKey(int key, int x, int y) {
  if (key == GLUT_KEY_F1) {
    showHelp = !showHelp;
    glutPostRedisplay();
  }
}

void mouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    float mx, my;
    if (intersectRayWithPlaneZ(x, y, -ROOM_SIZE + 0.1f, &mx, &my)) {
      const float centersX[3] = {-2, 0, 2};
      const float centerY = 1.5f;
      float w = 1.5f, h = 1.0f;
      selectedPainting = -1;
      for (int i = 0; i < 3; i++) {
        if (mx >= centersX[i] - w / 2 && mx <= centersX[i] + w / 2 &&
            my >= centerY - h / 2 && my <= centerY + h / 2) {
          selectedPainting = i;
          break;
        }
      }
    } else {
      selectedPainting = -1;
    }
    glutPostRedisplay();
  }
  if (button == GLUT_LEFT_BUTTON) {
    rotating = (state == GLUT_DOWN);
    lastX = x;
    lastY = y;
  }
}

void mouseMotion(int x, int y) {
  if (rotating) {
    yaw += (x - lastX) * 0.2f;
    pitch += (y - lastY) * 0.2f;
    lastX = x;
    lastY = y;
  }
  glutPostRedisplay();
}