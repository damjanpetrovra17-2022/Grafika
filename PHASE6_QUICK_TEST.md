# ?? Phase 6 — Quick Test Guide

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
Ray picker initialized for seat interaction  ? MORA biti
Test: Marked seats [2,5] and [0,0] as Purchased
```

---

## ??? Test Klikanja

### Test 1: Klikni Zeleno Sedište (OSNOVNI TEST)

1. **Pozicioniraj kameru** da vidiš sedišta
2. **Klikni levi miš** na **zeleno sedište** ??

**O?ekivano:**
- ? Sedište postaje **žuto** ?? (Reserved)
- ? Konzola: `Seat [r,c] -> Reserved`

---

### Test 2: Klikni Ponovo (Toggle Nazad)

1. **Klikni na žuto sedište** ?? koje si upravo rezervisao

**O?ekivano:**
- ? Sedište postaje **zeleno** ?? (Free)
- ? Konzola: `Seat [r,c] -> Free`

**State ciklus:** ?? ? ?? (Free ? Reserved)

---

### Test 3: Klikni Crveno Sedište (KRITI?AN TEST)

1. **Klikni na crveno sedište** ??
   - Row 2, Col 5 (sredina grid-a)
   - Row 0, Col 0 (prednji levi ugao)

**O?ekivano:**
- ? Sedište **ostaje crveno** ?? (ne menja se)
- ? Konzola: `Seat [r,c] is Purchased (cannot toggle)`

**State:** ?? ? ?? (Purchased se ne menja)

---

## ? Success Criteria

- [x] Build uspešan
- [x] Ray picker inicijalizovan
- [x] Zeleno ? Žuto na klik ?
- [x] Žuto ? Zeleno na drugi klik ?
- [x] Crveno ostaje crveno ?
- [x] Konzola loguje promene
- [x] Phase 0-5 features rade

**Ako je SVE ? ? Phase 6 DONE!**

---

## ?? Vizuelni Test

```
Pre klika:        Posle klika:
   ??????           ??????
   ??????    ?      ??????
   ??????           ??????
   
   ? Klikni       ? Žuto (Reserved)
   zeleno
```

**Test sekvencu:**
1. Klik 1: ?? ? ?? (Free to Reserved)
2. Klik 2: ?? ? ?? (Reserved to Free)
3. Klik 3: ?? ? ?? (Free to Reserved)
4. ... (može beskona?no)

**Crveno:**
- Klik: ?? ? ?? (bez promene)

---

## ?? Ako Nešto Ne Radi

### Problem: Klikovi se ne registruju
```powershell
# Proveri konzolu:
Ray picker initialized for seat interaction  ? Mora biti
```

**Rešenje:** Rebuild
```powershell
cmake --build build --config Debug --clean-first
```

---

### Problem: Sva sedišta se menjaju (i crvena)
**Bug:** State transition logika ne proverava Purchased

**Proveri:**
```cpp
// U Application::handleSeatPicking():
if (state == SeatState::Purchased)
    // Do nothing (already purchased)
```

---

### Problem: Klikovi ne odgovaraju mišu
**Mogu? uzrok:** NDC konverzija pogrešna

**Fix:** Proveri Y-flip
```cpp
y_ndc = 1.0f - (2.0f * mouseY) / screenHeight;
//      ? Mora biti 1.0f - ...
```

---

## ?? Kako Ray Picking Radi

```
Mouse (x, y)
    ?
1. Screen ? NDC
    ?
2. NDC ? Clip Space
    ?
3. Clip ? Eye Space (inverse projection)
    ?
4. Eye ? World Space (inverse view)
    ?
5. Normalize direction
    ?
Ray = (camera pos, direction)
    ?
6. Test all 50 seats (Ray vs AABB)
    ?
7. Pick closest hit
    ?
8. Toggle state
```

**Key:** **Najbliže** sedište se bira (smallest distance)

---

## ?? Kontrole (Iste kao Faza 2-5)

- **Miš** ? Look around + **Left Click** (NOVO!)
- **WASD / Strelice** ? Kretanje
- **ESC** ? Izlaz

**Novo:**
- **Left Mouse Click** ? Pick seat (Free ? Reserved)

---

## ?? Test Purchased Sedišta

**Dva test sedišta su marked as Purchased:**

| Sedište | Row | Col | State | Color |
|---------|-----|-----|-------|-------|
| Test 1 | 2 | 5 | Purchased | ?? Red |
| Test 2 | 0 | 0 | Purchased | ?? Red |

**Lokacija:**
- **[2,5]:** Srednji red, centar grid-a
- **[0,0]:** Prednji red, levi ugao

**Svrha:** Testirati da Purchased sedišta **ne mogu biti toggled**

---

**Slede?e:** Phase 7 — Purchase Logic & State Machine ??
