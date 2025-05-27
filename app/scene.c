#include "includes/scene.h"
#include "includes/textures.h"
#include "stb_image.h"

GLuint wallTex, floorTex, paintingTexture, paintingTexture2, paintingTexture3,
    ceilingTex, wallTex2, modelTex, helpTex;

void drawRoom(float s) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, wallTex);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-s, 0, -s);
  glTexCoord2f(1, 0);
  glVertex3f(s, 0, -s);
  glTexCoord2f(1, 1);
  glVertex3f(s, 2 * s, -s);
  glTexCoord2f(0, 1);
  glVertex3f(-s, 2 * s, -s);

  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-s, 0, s);
  glTexCoord2f(1, 0);
  glVertex3f(s, 0, s);
  glTexCoord2f(1, 1);
  glVertex3f(s, 2 * s, s);
  glTexCoord2f(0, 1);
  glVertex3f(-s, 2 * s, s);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, wallTex2);
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-s, 0, s);
  glTexCoord2f(1, 0);
  glVertex3f(-s, 0, -s);
  glTexCoord2f(1, 1);
  glVertex3f(-s, 2 * s, -s);
  glTexCoord2f(0, 1);
  glVertex3f(-s, 2 * s, s);
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(s, 0, -s);
  glTexCoord2f(1, 0);
  glVertex3f(s, 0, s);
  glTexCoord2f(1, 1);
  glVertex3f(s, 2 * s, s);
  glTexCoord2f(0, 1);
  glVertex3f(s, 2 * s, -s);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, ceilingTex);
  glBegin(GL_QUADS);
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-s, 2 * s, -s);
  glTexCoord2f(1, 0);
  glVertex3f(s, 2 * s, -s);
  glTexCoord2f(1, 1);
  glVertex3f(s, 2 * s, s);
  glTexCoord2f(0, 1);
  glVertex3f(-s, 2 * s, s);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, floorTex);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-s, 0, -s);
  glTexCoord2f(2, 0);
  glVertex3f(s, 0, -s);
  glTexCoord2f(2, 2);
  glVertex3f(s, 0, s);
  glTexCoord2f(0, 2);
  glVertex3f(-s, 0, s);
  glEnd();
}

void drawPainting(GLuint tex, float x, float y, float z) {
  stbi_set_flip_vertically_on_load(1);

  float w = 1.5f, h = 1.0f;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 1);
  glVertex3f(x - w / 2, y - h / 2, z);
  glTexCoord2f(1, 1);
  glVertex3f(x + w / 2, y - h / 2, z);
  glTexCoord2f(1, 0);
  glVertex3f(x + w / 2, y + h / 2, z);
  glTexCoord2f(0, 0);
  glVertex3f(x - w / 2, y + h / 2, z);

  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void drawPedestal(float x, float y, float z) {
  glColor3f(0.6f, 0.6f, 0.6f);
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(1, 0.5f, 1);
  glutSolidCube(1);
  glPopMatrix();
}

void drawModel(Mesh *m, float x, float y, float z) {
  const float yOffset = 1.05f;

  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(x, y - yOffset, z);

  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  float s = m->scale * 3.5f;
  glScalef(s, s, s);
  glTranslatef(-m->center.x, -m->center.y, -m->center.z);
  drawMesh(m, modelTex);
  glPopMatrix();
}