Virtuális Művészeti Galéria

Ez a projekt egy C nyelvű, OpenGL és GLUT alapokra épülő interaktív alkalmazás, amely lehetővé teszi, hogy első személyű nézetben sétálj végig egy virtuális művészeti galériában. A szobában festmények és szobrok (3D objektumok) várják a látogatót, és különféle vizuális effektek, világítás, köd és ütközésvizsgálat biztosítja a valósághű élményt.

🌟 Főbb funkciók

Első személyű kamera: folyamatos, sima mozgás (W/A/S/D), egérhúzással irányítás, sebességfüggő delta-idő alapú frissítés

Fények és köd: irányított fényforrás, állítható fényerő (+ / -), lineáris köd-effekt (fog) a tér mélységének érzékeltetéséhez

3D modellek: OBJ formátumú szobrok és festmények betöltése, egységesítés (unitize), textúrázás

Festmények: textúrázott quad-ok a falakon, kattintással nagyítva megtekinthetők (kinagyítás overlay)

Segítség panel: F1-gyel előhívható vezérlő- és információs panel, billentyűk és egér funkciók összefoglalója

Ütközésvizsgálat: AABB (axis-aligned bounding box) alapú, nem lehet áthaladni a pedálcsoportokon és a falakon

📋 Követelmények & függőségek

Fordító: clang vagy gcc (C99 kompatibilis)

Könyvtárak:

OpenGL

FreeGLUT / GLUT

stb_image (stb_image.h) képfájlok betöltéséhez

Platform: macOS

💻 Fordítás és futtatás

macOS

# Projekt gyökérkönyvtárban:
make
./virtual_gallery

Tisztítás:

make clean

🎮 Irányítás

Művelet

Billentyű / Egér

Mozgás előre / hátra

W / S

Mozgás balra / jobbra

A / D

Szabadnézet (look around)

Bal egérgomb + mozgatás

Fényerő növelése / csökkentése

+ / -

Segítségpanel

F1

Festmény kinagyítása

Bal egérgomb kattintás falon

Kilépés

ESC

📂 Projekt felépítése

virtual_gallery/
├── Makefile
├── README.md
├── main.c            # Fő forráskód
├── stb_image.h       # Képfájl betöltő könyvtár
├── assets/
   ├── textures/     # Festmény- és egyéb PNG/JPG textúrák
   └── models/       # OBJ modellek szobrokhoz

---