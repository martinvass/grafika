#define STB_IMAGE_IMPLEMENTATION
#include "includes/textures.h"
#include "stb_image.h"

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