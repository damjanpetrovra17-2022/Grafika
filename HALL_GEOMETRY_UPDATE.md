# ??? HALL GEOMETRY UPDATE — Stadium Platform & Stairs

## ? Šta Je Dodato/Promenjeno

**Promene:**
1. ? **Vrata pomerena** — Sa zadnjeg zida na **levi zid** (bliže ekranu)
2. ? **Stadium platforma** — Dodana platforma ispod sedišta
3. ? **Stepenice** — Centralne stepenice (5 stepena) izme?u leve i desne sekcije
4. ? **Prostor za hodnik** — Platforma je dovoljno široka za levi/desni hodnik

---

## ?? Izmene

### Izmenjeni Fajl
- ? `Source/Scene.cpp` — createHallGeometry() method

**Dodato:**
- Stadium platforma (1 objekat)
- Centralne stepenice (5 objekata)
- Vrata pomerena na levi zid

---

## ??? Nova Geometrija

### 1. Vrata (Door) — Premeštena

**PRE:**
- Pozicija: Zadnji zid (back wall, Z = +9)
- Centar sale

**POSLE:**
- Pozicija: **Levi zid** (left wall, X = -9)
- Bliže ekranu (Z = -5)

```cpp
// Door on left wall
Position: (-9 + wall_thickness, doorY, -5)
Size:     (0.05, doorHeight, doorWidth)  // Rotated for left wall
```

**Svrha:** Vrata su sada sa strane, ne blokiraju pogled na ekran.

---

### 2. Stadium Platforma (Platform)

**Nova geometrija:**
```cpp
Position: (0, floorY + 0.75, 3.5)  // Center of platform
Size:     (12, 1.5, 7)             // Width, Height, Depth
Color:    (0.3, 0.25, 0.2)         // Dark brown
```

**Karakteristike:**
- **Širina:** 12m — Dovoljno za sedišta + levi/desni hodnik
- **Visina:** 1.5m — Elevated platform (više od poda)
- **Dubina:** 7m — Pokriva 5 redova sedišta
- **Pozicija Z:** 3.5m — Centrirana iza sedišta (seat origin = 2.0)

**Svrha:**
- Sedišta ne "lebde" u vazduhu
- Vizualno razlikuje seating area od poda
- Stadium effect (platforma + stepenice)

---

### 3. Centralne Stepenice (Stairs)

**Nova geometrija:**
```cpp
// 5 steps (one per row)
for (int i = 0; i < 5; ++i)
{
    stepY = floorY + (i+1) * 0.3m
    stepZ = 0.5 + i * 1.2m
    
    Step size: (2.0, 0.3, 1.2)  // Width, Height, Depth
    Color: (0.28, 0.24, 0.22)   // Slightly darker brown
}
```

**Karakteristike:**
- **5 stepena** — Jedan po redu
- **Visina stepena:** 0.3m — Matches rowElevationStep
- **Dubina stepena:** 1.2m — Matches seatSpacingZ
- **Širina:** 2m — Central aisle between left/right sections
- **Pozicija:** Centar (X = 0) — Between seat sections

**Rezultat:**
```
Floor (Y=0.5)
  ?
Step 0: Y=0.8m  (floorY + 0.3)
Step 1: Y=1.1m  (floorY + 0.6)
Step 2: Y=1.4m  (floorY + 0.9)
Step 3: Y=1.7m  (floorY + 1.2)
Step 4: Y=2.0m  (floorY + 1.5)
  ?
Seats: Y=1.0..2.2m (stadium seating)
```

**Stepenice se poklapaju sa stadium seating elevacijom!**

---

## ?? Vizuelni Layout

### Top-Down View (Odozgo)
```
        SCREEN (Front Wall)
        ????????????????
        ?              ?
        ?              ?
  DOOR  ?              ?
    ??  ?              ?
 (Left  ?              ?
  Wall) ?              ?
        ?   PLATFORM   ?
        ?  ??????????  ?
        ?  ? ?????? ?  ? ? Seats (left section)
        ?  ?        ?  ?
        ?  ?  STAIRS?  ? ? Central stairs
        ?  ?        ?  ?
        ?  ? ?????? ?  ? ? Seats (right section)
        ?  ??????????  ?
        ?              ?
        ????????????????
```

---

### Side View (Sa Strane)
```
CEILING ???????????????????????
                      ?
                   SCREEN
                      ?
             ??    ? Row 4 (highest)
            ??     ? Row 3
           ??      ? Row 2
          ??       ? Row 1
         ??        ? Row 0 (lowest)
        ???        ? Stairs
       ????
      ?????
     ??????
    ???????
   ???????????     ? Platform
  ??????????????????? Floor
```

**Stadium effect sa stepenicama!**

---

### Front View (Od Ekrana)
```
        ?? (Door on left wall)
        ?
   ???????????
   ? ????????? ? ? Row 4
   ? ????????? ? ? Row 3
   ? ????????? ? ? Row 2
   ? ????????? ? ? Row 1
   ? ????????? ? ? Row 0
   ?     ?STAIRS?
   ?????????????
   
   Left   ?  Right
 Section  ? Section
          ?
    Central aisle
```

**Levi hodnik, centralne stepenice, desni hodnik.**

---

## ?? Dimenzije

### Platform (Platforma)
```
Width:  12m  (X: -6 to +6)
Height: 1.5m (Y: 0.5 to 2.0)
Depth:  7m   (Z: 0 to 7)
```

**Pokriva:**
- Svih 5 redova sedišta
- Levi hodnik (~1m)
- Desni hodnik (~1m)
- Centralne stepenice (2m)

---

### Stairs (Stepenice)
```
Width:       2m  (Central aisle)
Total Height: 1.5m (5 steps × 0.3m)
Total Depth:  6m  (5 steps × 1.2m)
Position:    X=0 (center), Z=0.5 to 6.5
```

**5 Stepena:**
| Step | Y Position | Z Position |
|------|-----------|-----------|
| 0 | 0.8m | 0.5m |
| 1 | 1.1m | 1.7m |
| 2 | 1.4m | 2.9m |
| 3 | 1.7m | 4.1m |
| 4 | 2.0m | 5.3m |

---

### Door (Vrata)
```
NEW Position: Left wall (X = -9), Z = -5
Size:         doorHeight=2.5m, doorWidth=2.0m
Orientation:  Rotated for left wall (thin X dimension)
```

**Premešteno sa zadnjeg zida na levi zid.**

---

## ? Test

### 1. Build i Pokreni
```powershell
cmake --build build --config Debug
.\out\build\x64-Debug\Debug\kostur.exe
```

### 2. Vizuelna Provera

**Trebalo bi da vidiš:**
- ? **Tamno-braon platforma** ispod sedišta (ne lebde više)
- ? **5 stepena** u sredini izme?u leve i desne sekcije
- ? **Vrata na levom zidu** (bliže ekranu)
- ? **Sedišta na platformi** (stadium seating)

---

### 3. Test Kretanja

**Test A: Hodnici**
1. **Idi levo od stepenica** (A key)
2. **Rezultat:** Levi hodnik izme?u zida i sedišta ?

**Test B: Centralne Stepenice**
1. **Idi ka stepenicama** (centar)
2. **Rezultat:** 5 stepena koji se penju ka zadnjim redovima ?

**Test C: Vrata**
1. **Idi ka levom zidu** (A key)
2. **Pogledaj ka ekranu** (mouse)
3. **Rezultat:** Vrata na levom zidu, bliže ekranu ?

---

## ?? Boje

### Platform & Stairs
```cpp
Platform: (0.3, 0.25, 0.2)   // Dark brown
Stairs:   (0.28, 0.24, 0.22) // Slightly darker brown
```

**Vizualno se razlikuju ali su harmonizirane.**

---

## ?? Ako Treba Podešavanje

### Podigni Platformu
```cpp
// U Scene.cpp:
const float platformHeight = 2.0f;  // Was 1.5f
```

---

### Šira Platforma (Ve?i Hodnici)
```cpp
const float platformWidth = 14.0f;  // Was 12.0f
```

---

### Šire Stepenice
```cpp
const float stepWidth = 3.0f;  // Was 2.0f
```

---

### Pomeri Vrata
```cpp
// Bliže/dalje od ekrana:
const float doorZ = -3.0f;  // Was -5.0f (closer to screen)
```

---

## ?? Struktura Scene

**Redosled objekata:**
1. Floor
2. Ceiling
3. Front Wall (screen wall)
4. Back Wall
5. Left Wall (with door)
6. Right Wall
7. Screen (unlit)
8. **Door (on left wall)** ? NOVO mesto
9. **Platform** ? NOVO
10. **Stairs (5 steps)** ? NOVO

**Ukupno:** 13 objekata (bilo 8, dodato 6)

---

## ? Success Criteria

- [x] Vrata na levom zidu ?
- [x] Platforma ispod sedišta ?
- [x] 5 stepena u sredini ?
- [x] Sedišta ne lebde ?
- [x] Hodnici sa strane ?
- [x] Build uspešan ?

**SVE DONE! ?**

---

## ?? Rezultat

**Sada imaš:**
- ? **Realisti?nu stadium geometriju** — Platforma + stepenice
- ? **Vrata na levom zidu** — Ne blokiraju pogled
- ? **Centralne stepenice** — Pristup zadnjim redovima
- ? **Hodnici** — Levi i desni prolaz
- ? **Stadium seating** — Sedišta na platformi

**Bioskop izgleda mnogo realisti?nije!** ?????

---

**Status:** ? **COMPLETE**  
**Build:** ? **Successful**  
**Visual:** ? **Stadium Platform & Stairs Added**

---

**Dodato:** Platform + Stairs + Door relocation  
**LOC:** ~50 novih linija  
**Objects Added:** 6 (1 platform + 5 stairs)
