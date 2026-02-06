# ? PHASE 7 IMPLEMENTATION — Summary

## ?? Status: COMPLETE

**Phase 7 — Purchase Logic (Group Seating)** je uspešno implementirana!

---

## ?? Šta Je Dodato

### Izmenjeni Fajlovi (4)
1. ? `Header/SeatGrid.h` — purchaseAdjacent() method
2. ? `Source/SeatGrid.cpp` — Group seating logic (~50 linija)
3. ? `Header/Application.h` — handlePurchaseKeys() method
4. ? `Source/Application.cpp` — Keys 1-9 handling (~60 linija)

**Dokumentacija:**
- ? `PHASE7_COMPLETE.md` — Full completion report

**Ukupno linija koda:** ~150 novih linija

---

## ?? Šta Radi

### Group Seating (Grupno Sedenje)

**KLJU?NA FUNKCIONALNOST:**
- Grupa od N ljudi MORA sedeti ZAJEDNO
- U **istom redu**
- **Jedno pored drugog** (adjacent seats)
- **BEZ razdvajanja** preko više redova

**Kao u pravom bioskopu!** ??

---

### Keys 1-9 ? Kupi N Sedišta

```
Pritisni 1 ? Kupi 1 sedište
Pritisni 2 ? Kupi 2 sedišta (jedno pored drugog)
Pritisni 3 ? Kupi 3 sedišta (sva u istom redu)
...
Pritisni 9 ? Kupi 9 sedišta (sva adjacent)
```

**Edge-triggered:** Jedan purchase po pritisku tastera.

---

### Scan Order (Redosled Pretrage)

**KRITI?NO — Mora biti ovako:**
1. **Redovi:** Poslednji (4) ? Prvi (0) (zadnji redovi prvo)
2. **Kolone:** Desno (9) ? Levo (0) (desna strana prvo)
3. **Prvi validan blok** ? Bira se

```cpp
for (row = 4; row >= 0; --row)      // Zadnji ? Prvi
    for (col = 9; col >= N-1; --col)  // Desno ? Levo
        if (N consecutive Free seats)
            Purchase all N ? DONE
```

---

### Pravila (Validation)

**? Validan blok:**
```
[Free][Free][Free]  ? 3 Free seats, adjacent
```

**? Invalidan blok:**
```
[Free][Reserved][Free]  ? Reserved breaks continuity
[Free][Purchased][Free] ? Purchased breaks continuity
[Free][Free]            ? Only 2 seats (need 3)
```

**Reserved i Purchased BLOKIRAJU grupno sedenje.**

---

## ?? Kako Testirati

### 1. Build i Pokreni
```powershell
cmake --build build --config Debug
.\out\build\x64-Debug\Debug\kostur.exe
```

### 2. Test Osnovne Kupovine

#### Test A: Kupi 1 Sedište
1. **Pritisni 1**
2. **Rezultat:**
   - Zadnji red, najdesnije Free sedište ? Crveno (Purchased)
   - Konzola: `Purchased group of 1 seats in row 4, cols 9..9`

---

#### Test B: Kupi 3 Sedišta
1. **Pritisni 3**
2. **Rezultat:**
   - 3 sedišta **jedno pored drugog** ? Crveno
   - **Isti red**
   - Konzola: `Purchased group of 3 seats in row X, cols Y..Z`

**Proveri:** Sva 3 sedišta su **adjacent** (bez praznina) ?

---

### 3. Test Blokiranja (Reserved)

#### Test C: Fragmentiraj Red
1. **Klikni na sedište** da ga rezervišeš (žuto)
2. Napravi ovaj pattern u redu:
   ```
   [Free][Free][Reserved][Free][Free]
   ```

3. **Pritisni 3** (kupi 3)

**Rezultat:**
- Taj red se **preska?e** (nema 3 consecutive Free)
- Slede?i red sa 3 Free se bira
- Konzola: `Purchased group of 3 seats in row Y...` (drugi red)

**Reserved blokira grupno sedenje!** ?

---

#### Test D: Pokušaj Kupiti Previše
1. **Fragmentiraj sve redove** da nijedan nema 6+ Free
2. **Pritisni 6**

**Rezultat:**
- Kupovina **ne uspeva**
- Konzola: `Cannot seat group of 6 together (no adjacent block found)`
- **Nijedno sedište se ne menja** (all-or-nothing)

**Parcijalna kupovina NE POSTOJI!** ?

---

### 4. Test Scan Order-a

#### Test E: Zadnji Red Prvo
1. **Osiguraj da svi redovi imaju Free sedišta**
2. **Pritisni 2**

**Rezultat:**
- **Red 4 (zadnji)** se bira prvo
- Konzola: `...in row 4...`

**Scan po?inje od zadnjeg reda!** ?

---

#### Test F: Desno ? Levo
1. **Pogledaj zadnji red**
2. **Pritisni 1**

**Rezultat:**
- **Najdesnije Free sedište** se kupuje
- Vizuelno: Crveno sedište na desnoj strani

**Scan ide s desna na levo!** ?

---

## ?? Group Seating Algoritam

### Pseudocode

```
purchaseAdjacent(N):
    if N < 1 or N > 9 or N > COLS:
        return false
    
    for row from (ROWS-1) down to 0:
        for col from (COLS-1) down to (N-1):
            // Check N consecutive seats
            allFree = true
            for i from 0 to (N-1):
                if seats[row][col-i] != Free:
                    allFree = false
            
            if allFree:
                // Purchase all N
                for i from 0 to (N-1):
                    seats[row][col-i] = Purchased
                return true
    
    return false  // No valid block
```

---

### Primer Izvršavanja (N=3)

**Stanje:**
```
Row 4: [F][F][F][F][F][F][F][F][F][F]  ? Start
Row 3: [F][F][P][F][F][F][F][F][F][F]
Row 2: [F][F][F][R][F][F][F][F][F][F]
...
```

**Scan:**
```
1. Row 4, col 9: Check [9,8,7] ? All Free ?
   ? Purchase [4,9], [4,8], [4,7]
   ? DONE
```

**Rezultat:**
```
Row 4: [F][F][F][F][F][F][F][P][P][P]  ? Grupa!
```

**Grupa od 3 je seden? zajedno u redu 4, kolone 7..9.**

---

## ? Acceptance Criteria

- [x] Grupe uvek u istom redu ?
- [x] Nikad razdvajanje preko redova ?
- [x] Adjacent seats (bez praznina) ?
- [x] Reserved/Purchased blokira grupisanje ?
- [x] All-or-nothing purchase ?
- [x] Scan: zadnji red ? prvi red ?
- [x] Scan: desno ? levo ?
- [x] Keys 1-9 rade ?
- [x] Edge cases handled ?
- [x] Build uspešan ?
- [x] Phase 0-6 features rade ?

**SVE DONE! ?**

---

## ?? Slede?a Faza

**Phase 8 — People Spawning & Animation** (Slede?e)

**Šta treba:**
1. Person class (3D model)
2. Spawn people on Purchased seats
3. Walking animation (ka sedištu)
4. Sitting animation
5. Person disappears after sitting

**Procena:** 8-12 sati

---

## ?? Progres Projekta

| Faza | Status |
|------|--------|
| Phase 0 | ? DONE |
| Phase 1 | ? DONE |
| Phase 2 | ? DONE |
| Phase 3 | ? DONE |
| Phase 4 | ? DONE |
| Phase 5 | ? DONE |
| Phase 6 | ? DONE |
| **Phase 7** | ? **DONE** |
| Phase 8-13 | ? TODO |

**Progres:** 62% (8/13 faza)  
**Preostalo:** ~24-42 sata rada

---

## ? Rezime

**Dodato:**
- Group seating logic (purchaseAdjacent)
- Keys 1-9 za kupovinu N sedišta
- Scan order: zadnji red prvo, desno ? levo
- Validation: Reserved/Purchased breaks blocks
- All-or-nothing transaction

**Nije promenjeno:**
- Phase 0-6 moduli
- Seat clicking (Free ? Reserved)
- Ray picking
- Stadium seating

**Rezultat:**
- Realisti?an bioskop sistem
- Grupe uvek sede zajedno
- Bez razdvajanja, bez praznina
- Spremno za spawning ljudi (Phase 8)

---

**Vreme implementacije:** ~2-3 sata  
**Build status:** ? Successful  
**Ready for:** Phase 8 — People Spawning

---

Pokreni aplikaciju i kupi grupe sedišta! ?????
