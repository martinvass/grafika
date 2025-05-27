# Virtuális Művészeti Galéria

Ez a projekt egy C nyelven írt, OpenGL/GLUT alapokra épülő interaktív alkalmazás, ahol első-személyű nézetben barangolhatsz egy virtuális művészeti galériában. A térben festmények és 3D-szobrok várnak felfedezésre, a valósághű élményt pedig fény-árnyék effektusok, lineáris köd és AABB-alapú ütközésvizsgálat teszi teljessé.

---

## 🌟 Főbb funkciók

- **Első személyű kamera**:  
  W/A/S/D mozgás, egérrel nézelődés, delta‐idő alapú sima frissítés  
- **Fények & köd**:  
  Állítható fényerő (+ / –), lineáris köd a mélység érzékeltetéséhez  
- **3D modellek**:  
  OBJ fájlok betöltése, egységesítés (_unitize_), normál‐ és textúra koordináták  
- **Festmények**:  
  Textúrázott quad-ok a falakon, kattintással fullscreen overlay  
- **Ütközésvizsgálat**:  
  A falak és kiállítópedálok körül AABB blokkolás  

---

## 📋 Követelmények & függőségek

- **Fordító**: clang vagy gcc (C99 támogatással)  
- **Könyvtárak**:  
  - OpenGL  
  - FreeGLUT / GLUT  
  - stb_image (`stb_image.h`)  
- **Platform**: macOS  

---

## 💻 Fordítás és futtatás

```bash
# Projekt gyökérkönyvtárban:
make
./virtual_gallery
```

**Takarítás**:
```bash
make clean
```

---

## 🎮 Irányítás

| Művelet                          | Billentyű / Egér                  |
|----------------------------------|-----------------------------------|
| Mozgás előre / hátra             | W / S                             |
| Mozgás balra / jobbra            | A / D                             |
| Szabad nézet (look around)       | Bal egérgomb + mozgatás           |
| Fényerő növelése / csökkentése   | + / –                             |
| Segítségpanel (overlay)          | F1                                |
| Festmény kinagyítása (overlay)   | Bal egérgomb kattintás falon      |
| Kilépés                          | ESC                               |

---

## 📂 Projekt felépítése
```
virtual_gallery/
├── Makefile
├── README.md
├── assets/
│   ├── textures/     # PNG/JPG textúrák
│   └── models/       # OBJ 3D modellek
├── build/            # .o build fájlok
│   └── *.o
├── includes/         # Publikus header fájlok
│   ├── camera.h
│   ├── input.h
│   ├── mesh.h
│   ├── render.h
│   ├── scene.h
│   ├── stb_image.h
│   ├── textures.h
│   └── types.h
├── camera.c
├── input.c
├── main.c
├── mesh.c
├── render.c
├── scene.c
├── textures.c
└── types.c
```