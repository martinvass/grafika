#include "includes/render.h"
#include "includes/scene.h"
#include "includes/camera.h"

bool showHelp = false;
int selectedPainting = -1;

Mesh *model1, *model2;

void display() {
  static int lastTime = 0;
  int current = glutGet(GLUT_ELAPSED_TIME);
  float delta = (current - lastTime) / 1000.0f;
  lastTime = current;

  updateCamera(delta);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(-pitch, 1, 0, 0);
  glRotatef(-yaw, 0, 1, 0);
  glTranslatef(-camX, -camY, -camZ);

  glColor3f(brightness, brightness, brightness);

  drawRoom(ROOM_SIZE);
  drawPainting(paintingTexture, -2, 1.5f, -ROOM_SIZE + 0.1f);
  drawPainting(paintingTexture2, 0, 1.5f, -ROOM_SIZE + 0.1f);
  drawPainting(paintingTexture3, 2, 1.5f, -ROOM_SIZE + 0.1f);

  drawPedestal(-2, 0.25, 2);
  drawModel(model1, -2, 3.35, 2);
  drawPedestal(2, 0.25, 2);
  drawModel(model2, 2, 3.35, 2);

  glColor3f(1, 1, 1);

  if (showHelp) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, helpTex);
    glColor3f(1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float margin = 50.0f;
    float imgW = w - 2 * margin, imgH = h - 2 * margin;
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(margin, margin);
    glTexCoord2f(1, 1);
    glVertex2f(margin + imgW, margin);
    glTexCoord2f(1, 0);
    glVertex2f(margin + imgW, margin + imgH);
    glTexCoord2f(0, 0);
    glVertex2f(margin, margin + imgH);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  if (selectedPainting >= 0) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLuint tex = (selectedPainting == 0   ? paintingTexture
                  : selectedPainting == 1 ? paintingTexture2
                                          : paintingTexture3);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(0, 0);
    glTexCoord2f(1, 1);
    glVertex2f(w, 0);
    glTexCoord2f(1, 0);
    glVertex2f(w, h);
    glTexCoord2f(0, 0);
    glVertex2f(0, h);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
    return;
  }

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (float)w / h, 0.1, 100);
  glMatrixMode(GL_MODELVIEW);
}