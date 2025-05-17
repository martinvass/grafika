# Virtuális Művészeti Galéria

Egy C, OpenGL, SDL2 és GLUT alapú egyszerű projekt, amely lehetővé teszi, hogy első személyű nézetben sétálj egy virtuális művészeti galériában.

Megtekinthetőek festmények és szobrok, ezekkel interaktálni is lehet egyaránt.

---

## Főbb funkciók

* **Első személyű kamera**: WASD + egér nézet mozgatás
* **3D modellek**: OBJ betöltő szobrokhoz, festményekhez
* **Fények**: irányított és spot fényforrások, `+`/`-` a fényerő állításához
* **Segítség (help) panel**: `F1`-gyel előhívható vezérlő- és információs átfedés
* **Interaktálás szobrokkal**: A szobrokra kattintva azok forgathatóak

---

## Fordítás és futtatás

### macOS

```bash
make macos
./beadando
```

### Windows

```bash
make all
beadando.exe
```

---

## Vezérlés

| Művelet                       | Billentyű / Egér     |
| ----------------------------- | -------------------- |
| Előre / hátra mozgás          | W / S                |
| Balra / jobbra mozgás         | A / D                |
| Fel / le mozogás              | Space / C            |
| Fényerő növelése              | +                    |
| Fényerő csökkentése           | -                    |
| Segítségpanelen megjelenítése | F1                   |

---