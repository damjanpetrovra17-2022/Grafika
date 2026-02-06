# ?? Phase 5 — Quick Test Guide

## ? Brzi Test (3 minuta)

### 1. Build
```powershell
cmake --build build --config Debug
```

### 2. Pokreni
```powershell
.\out\build\x64-Debug\Debug\kostur.exe
```

### 3. Proveri Konzolu
```
Seat grid initialized (5x10 seats with stadium seating)  ? MORA biti
[INFO] Application initialized successfully
```

### 4. Vizuelna Provera

**Odmah po pokretanju trebalo bi da vidiš:**

```
     PLAFON
    ???????????????????????????
    ?                         ?
    ?   EKRAN (BEL)           ? ? Front wall
    ?   ????????              ?
    ?                         ?
    ?   ????????????????????  ? ? Row 0 (najniži)
    ?                         ?
    ?   ????????????????????  ? ? Row 1
    ?                         ?
    ?   ????????????????????  ? ? Row 2
    ?                         ?
    ?   ????????????????????  ? ? Row 3
    ?                         ?
    ?   ????????????????????  ? ? Row 4 (najviši)
    ?                         ?
    ?   ?? KAMERA             ?
    ???????????????????????????
```

? **AKO VIDIŠ:**
- **50 zelenih kocki** (5 redova × 10 kolona)
- Sedišta u **centru sale**, okrenuta ka ekranu
- **Zadnji redovi VIŠI** od prednjih (stadium effect)
- Sedišta su **osvetljena** (Phong lighting)

? **RADI! Phase 5 je OK!** ?

? **AKO VIDIŠ:**
- Nema sedišta ? SeatGrid nije inicijalizovan
- Sva sedišta na istoj visini ? rowElevationStep = 0 (greška)
- Sedišta van sale ? pogrešan origin

---

## ?? Brzi Stadium Seating Test

### Test 1: Bo?ni Pogled (KRITI?AN TEST)
1. **Strafe levo** (pritisni **A**)
2. Gledaj sedišta **sa strane**

**O?ekivano:**
```
    ??  ? Row 4 (2.2m) najviši
   ??   ? Row 3 (1.9m)
  ??    ? Row 2 (1.6m)
 ??     ? Row 1 (1.3m)
??      ? Row 0 (1.0m) najniži

? Jasno vidljiv USPON (stadium slope)
```

? **Ako vidiš nagib** ? Stadium seating radi!  
? **Ako je ravno** ? rowElevationStep nije primenjen

---

### Test 2: Prednji Pogled
1. **Idi ka ekranu** (pritisni **W**)
2. **Okreni se** ka sedištima (miš)

**O?ekivano:**
- Redovi su **slojevito postavljeni** (jedan iznad drugog)
- Zadnji red **vidljivo viši**

---

### Test 3: Kretanje Kroz Redove
1. Kreni ka prvom redu (**W**)
2. Produži ka poslednjem redu

**O?ekivano:**
- Svaki slede?i red je **malo viši**
- Ose?aš **blagi uspon** dok hodiš

---

## ? Success Criteria

- [x] Build uspešan
- [x] 50 sedišta vidljivo (5×10)
- [x] Sva sedišta **zelena** (Free state)
- [x] Zadnji redovi **VIŠI** od prednjih
- [x] Nagib je **blagi** (ne stepenište)
- [x] Sedišta **osvetljena** (Phong lighting)
- [x] Sedišta **unutar hall bounds**
- [x] Phase 0-4 features rade

**Ako je SVE ? ? Phase 5 DONE!**

---

## ?? Ako Nešto Ne Radi

### Problem: Nema sedišta (crna sala)
```powershell
# Proveri konzolu:
# Trebalo bi da kaže:
Seat grid initialized (5x10 seats with stadium seating)
```

**Rešenje:** Rebuild clean
```powershell
cmake --build build --config Debug --clean-first
```

---

### Problem: Sedišta ravna (nema nagiba)
```cpp
// Proveri parametar u Application::init():
float rowElevationStep = 0.3f;  // MORA biti > 0
```

**Test:** Pogledaj sa strane (strafe levo) — trebalo bi da vidiš nagib

---

### Problem: Sedišta van sale
```cpp
// Podesi origin u Application::init():
glm::vec3 seatOrigin(0.0f, 1.0f, 2.0f);
// X=0 (centrano), Y=1.0 (iznad poda), Z=2.0 (sredina-nazad)
```

---

## ?? Stadium Seating Parametri

```cpp
// U Application::init():
glm::vec3 seatOrigin(0.0f, 1.0f, 2.0f);
float seatSpacingX = 1.0f;        // Razmak levo-desno
float seatSpacingZ = 1.2f;        // Razmak napred-nazad
float rowElevationStep = 0.3f;    // ELEVACIJA PO REDU
```

**Elevacija:**
- Row 0: 1.0m
- Row 1: 1.3m
- Row 2: 1.6m
- Row 3: 1.9m
- Row 4: 2.2m

**Ukupan rise:** 1.2m (realisti?no za bioskop)

---

**Slede?e:** Phase 6 — Seat Interaction (Ray Picking) ?????
