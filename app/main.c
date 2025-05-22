#include <GLUT/glut.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float brightness = 1.0f;
const float brightnessStep = 0.1f;

float camX = 0.0f, camY = 1.6f, camZ = 4.0f;
float yaw = 0.0f, pitch = 0.0f;
int lastX = 0, lastY = 0;
bool movingForward = false, movingBackward = false, movingLeft = false,
     movingRight = false, rotating = false;
float speed = 3.0f;

const float ROOM_SIZE = 5.0f;

GLuint wallTex, floorTex, paintingTexture, paintingTexture2, paintingTexture3,
    ceilingTex, wallTex2, modelTex;

GLuint helpTex;
bool showHelp = false;
int selectedPainting = -1;

typedef struct {
  float x, y, z;
} Vec3;

typedef struct {
  Vec3 *verts;
  int vertCount;
  Vec3 *norms;
  int normCount;
  int *vIndex;
  int vIndexCount;
  int *nIndex;
  int nIndexCount;
  int faceCount;
  Vec3 center;
  float scale;
} Mesh;

typedef struct {
  float minX, maxX, minZ, maxZ;
} Box;
Box obstacles[] = {
    {-2.5f, -1.5f, 1.5f, 2.5f},
    {1.5f, 2.5f, 1.5f, 2.5f}
};
int obstacleCount = sizeof(obstacles) / sizeof(Box);

Mesh *model1, *model2;

void unitizeMesh(Mesh *m) {
  float minX = 1e9f, minY = 1e9f, minZ = 1e9f;
  float maxX = -1e9f, maxY = -1e9f, maxZ = -1e9f;
  for (int i = 0; i < m->vertCount; i++) {
    Vec3 v = m->verts[i];
    if (v.x < minX) minX = v.x;
    if (v.x > maxX) maxX = v.x;
    if (v.y < minY) minY = v.y;
    if (v.y > maxY) maxY = v.y;
    if (v.z < minZ) minZ = v.z;
    if (v.z > maxZ) maxZ = v.z;
  }
  m->center.x = (minX + maxX) * 0.5f;
  m->center.y = (minY + maxY) * 0.5f;
  m->center.z = (minZ + maxZ) * 0.5f;
  float dx = maxX - minX, dy = maxY - minY, dz = maxZ - minZ;
  float maxd = fmaxf(dx, fmaxf(dy, dz));
  m->scale = 1.0f / maxd;
}

static void appendVec3(Vec3 **arr, int *count, float x, float y, float z) {
  *arr = realloc(*arr, sizeof(Vec3) * (*count + 1));
  (*arr)[(*count)++] = (Vec3){x, y, z};
}
static void appendInt(int **arr, int *count, int v) {
  *arr = realloc(*arr, sizeof(int) * (*count + 1));
  (*arr)[(*count)++] = v;
}

Mesh *loadOBJ(const char *path) {
  FILE *f = fopen(path, "r");
  if (!f) {
    perror(path);
    exit(1);
  }

  Mesh *m = calloc(1, sizeof(Mesh));
  char line[256];

  while (fgets(line, sizeof(line), f)) {
    if (line[0] == 'v' && line[1] == ' ') {
      float x, y, z;
      sscanf(line + 2, "%f %f %f", &x, &y, &z);
      appendVec3(&m->verts, &m->vertCount, x, y, z);
    }
    else if (line[0] == 'v' && line[1] == 'n') {
      float x, y, z;
      sscanf(line + 2, "%f %f %f", &x, &y, &z);
      appendVec3(&m->norms, &m->normCount, x, y, z);
    }
    else if (line[0] == 'f' && line[1] == ' ') {
      char *tok = strtok(line + 2, " \t\r\n");
      int faceVerts = 0;

      while (tok && faceVerts < 3) {
        int vi, ni;
        if (sscanf(tok, "%d//%d", &vi, &ni) == 2) {
        }
        else if (sscanf(tok, "%d/%*d/%d", &vi, &ni) == 2) {
        } else {
          fprintf(stderr, "Unsupported face format: %s\n", tok);
          exit(1);
        }

        appendInt(&m->vIndex, &m->vIndexCount, vi - 1);
        appendInt(&m->nIndex, &m->nIndexCount, ni - 1);

        faceVerts++;
        tok = strtok(NULL, " \t\r\n");
      }

      if (faceVerts == 3) {
        m->faceCount++;
      }
    }
  }

  fclose(f);
  return m;
}

void drawMesh(const Mesh *m, GLuint tex) {
  glBindTexture(GL_TEXTURE_2D, tex);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < m->vIndexCount; ++i) {
    Vec3 n = m->norms[m->nIndex[i]];
    glNormal3f(n.x, n.y, n.z);

    Vec3 v = m->verts[m->vIndex[i]];
    float u = (v.x - (m->center.x - 0.5f / m->scale)) * m->scale;
    float w = (v.z - (m->center.z - 0.5f / m->scale)) * m->scale;
    glTexCoord2f(u, w);

    glVertex3f(v.x, v.y, v.z);
  }
  glEnd();
}

GLuint genSolid(int r, int g, int b) {
  unsigned char data[3] = {(unsigned char)r, (unsigned char)g,
                           (unsigned char)b};
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  return tex;
}

GLuint genChecker(int w, int h, unsigned char c1[3], unsigned char c2[3]) {
  unsigned char *data = malloc(3 * w * h);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int i = 3 * (y * w + x);
      unsigned char *c = (((x / 4 + y / 4) % 2) ? c1 : c2);
      data[i] = c[0];
      data[i + 1] = c[1];
      data[i + 2] = c[2];
    }
  }
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  free(data);
  return tex;
}

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
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(x, y, z);

  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  float s = m->scale * 5.5f;
  glScalef(s, s, s);
  glTranslatef(-m->center.x, -m->center.y, -m->center.z);
  drawMesh(m, modelTex);
  glPopMatrix();
}

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

void specialKey(int key, int x, int y) {
  if (key == GLUT_KEY_F1) {
    showHelp = !showHelp;
    glutPostRedisplay();
  }
}

void keyUp(unsigned char key, int x, int y) {
  if (key == 'w') movingForward = false;
  if (key == 's') movingBackward = false;
  if (key == 'a') movingLeft = false;
  if (key == 'd') movingRight = false;
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

GLuint importPainting(const char *paintingName) {
  int pw, ph, pc;
  unsigned char *pdata = stbi_load(paintingName, &pw, &ph, &pc, 0);
  if (!pdata) {
    fprintf(stderr, "Cannot load %s\n", paintingName);
    exit(1);
  }
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  GLenum fmt = (pc == 4 ? GL_RGBA : GL_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, pw, ph, 0, fmt, GL_UNSIGNED_BYTE, pdata);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  stbi_image_free(pdata);
  return tex;
}

void initModels() {
  model1 = loadOBJ("assets/models/model1.obj");
  unitizeMesh(model1);

  model2 = loadOBJ("assets/models/model2.obj");
  unitizeMesh(model2);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Virtual Gallery");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  GLfloat fogColor[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, 5.0f);
  glFogf(GL_FOG_END, 15.0f);
  glHint(GL_FOG_HINT, GL_NICEST);
  glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  paintingTexture = importPainting("assets/textures/painting.jpg");
  paintingTexture2 = importPainting("assets/textures/painting2.jpg");
  paintingTexture3 = importPainting("assets/textures/painting3.jpg");
  modelTex = importPainting("assets/textures/texture.png");

  helpTex = importPainting("assets/textures/help.png");

  glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);
  glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

  unsigned char c1[3] = {200, 180, 140}, c2[3] = {210, 190, 160};
  unsigned char f1[3] = {120, 70, 20}, f2[3] = {160, 100, 40};
  wallTex = genChecker(4, 4, c1, c2);
  floorTex = genChecker(16, 16, f1, f2);
  wallTex2 = genSolid(150, 20, 20);
  ceilingTex = genSolid(20, 40, 60);

  initModels();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyDown);
  glutSpecialFunc(specialKey);
  glutKeyboardUpFunc(keyUp);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMotion);
  glutIdleFunc(display);

  glutMainLoop();
  return 0;
}
