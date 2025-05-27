#include <GLUT/glut.h>
#include "includes/types.h"
#include "includes/mesh.h"
#include "includes/render.h"
#include "includes/textures.h"
#include "includes/scene.h"
#include "includes/camera.h"
#include "includes/input.h"

void initModels() {
  model1 = loadOBJ("assets/models/model1.obj");
  unitizeMesh(model1);

  model2 = loadOBJ("assets/models/model2.obj");
  unitizeMesh(model2);
}

void initGL(void) {
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

  glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);

  glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Virtuális Galéria");

  initGL();

  paintingTexture = importPainting("assets/textures/painting.jpg");
  paintingTexture2 = importPainting("assets/textures/painting2.jpg");
  paintingTexture3 = importPainting("assets/textures/painting3.jpg");
  modelTex = importPainting("assets/textures/texture.png");
  helpTex = importPainting("assets/textures/help.png");

  unsigned char c1[3] = {200, 180, 140}, c2[3] = {210, 190, 160};
  unsigned char f1[3] = {120, 70, 20}, f2[3] = {160, 100, 40};
  wallTex = genChecker(4, 4, c1, c2);
  floorTex = genChecker(16, 16, f1, f2);
  wallTex2 = genSolid(158, 100, 55);
  ceilingTex = genSolid(96, 96, 96);

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