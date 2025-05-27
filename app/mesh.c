#include "includes/mesh.h"

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