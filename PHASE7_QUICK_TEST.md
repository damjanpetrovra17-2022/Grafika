# ?? Phase 7 — Quick Test Guide

## ? Brzi Test (3 minuta)

### 1. Build
```powershell
cmake --build build --config Debug
```

### 2. Pokreni
```powershell
.\out\build\x64-Debug\Debug\kostur.exe
```

---

## ?? Test Keys 1-9

### Test 1: Kupi 1 Sedište (OSNOVNI TEST)

1. **Pritisni taster 1**

**O?ekivano:**
- ? Jedno sedište postaje **crveno** ?? (Purchased)
- ? Konzola: `Purchased group of 1 seats in row 4, cols 9..9`
- ? Zadnji red (4), desna strana (col 9)

**RADI!** ?

---

### Test 2: Kupi 3 Sedišta (GROUP TEST)

1. **Pritisni taster 3**

**O?ekivano:**
- ? **Tri sedišta jedno pored drugog** postaju crvena ??????
- ? Sva **u istom redu**
- ? Konzola: `Purchased group of 3 seats in row X, cols Y..Z`

**Proveri:** Sedišta su **adjacent** (bez praznina izme?u) ?

---

### Test 3: Fragmentiraj Red (BLOCKING TEST)

#### Korak 1: Rezerviši Sedište
1. **Pozicioniraj crosshair** na sedište u sredini reda
2. **Left Click** ? Sedište postaje **žuto** ?? (Reserved)

#### Korak 2: Pokušaj Kupiti Grupu
1. **Pritisni 3** (kupi 3 sedišta)

**O?ekivano:**
- ? Red sa Reserved se **preska?e**
- ? Drugi red sa 3 Free se bira
- ? Konzola: `Purchased group of 3 seats in row Y...` (drugi red, ne fragmentirani)

**Reserved BLOKIRA grupisanje!** ?

---

### Test 4: Previše Velika Grupa (FAILURE TEST)

#### Korak 1: Fragmentiraj Sve Redove
1. **Rezerviši sedišta** da nijedan red nema 7+ Free
   - Klikaj po sredini svakog reda

#### Korak 2: Pokušaj Kupiti 7
1. **Pritisni 7**

**O?ekivano:**
- ? Kupovina **ne uspeva**
- ? Konzola: `Cannot seat group of 7 together (no adjacent block found)`
- ? **Nijedno sedište se ne menja** (all-or-nothing)

**Parcijalna kupovina NE POSTOJI!** ?

---

## ? Success Criteria

- [x] Build uspešan
- [x] Key 1 kupuje 1 sedište ?
- [x] Key 3 kupuje 3 adjacent sedišta ?
- [x] Grupe uvek u istom redu ?
- [x] Reserved blokira grupisanje ?
- [x] Failure kada nema bloka ?
- [x] Phase 0-6 features rade

**Ako je SVE ? ? Phase 7 DONE!**

---

## ?? Vizuelni Test

### Pre kupovine:
```
Row 4: ????????????????????
Row 3: ????????????????????
...
```

### Posle (Pritisni 3):
```
Row 4: ????????????????????  ? Grupa ovde!
Row 3: ????????????????????
...
```

**Tri crvena sedišta, jedno pored drugog, isti red!** ?

---

### Sa Reserved (Pritisni 4):
```
Row 4: ????????????????????  ? Reserved blokira
       ??????                    ? Samo 2 Free (ne može 4)
              ????????????????   ? 7 Free (može 4!)
```

**Rezultat:**
```
Row 4: ????????????????????  ? Grupa desno od Reserved
```

**Reserved se preska?e, grupa nastavlja s desna!** ?

---

## ?? Ako Nešto Ne Radi

### Problem: Grupa se razdvaja preko redova
**Proveri:** purchaseAdjacent() — sva sedišta u istom `row`

---

### Problem: Reserved sedište uklju?eno u grupu
**Proveri:** Validation logic — `if (state != Free) ? break`

---

### Problem: Kupovina ne uspeva ali bi trebala
**Proveri:** Scan order — desno ? levo, zadnji ? prvi

---

## ?? Kontrole (Kompletan Pregled)

| Kontrola | Akcija |
|----------|--------|
| **1-9** | Kupi N adjacent sedišta (NOVO!) |
| **Mouse** | Look around |
| **Left Click** | Pick seat (Free ? Reserved) |
| **WASD** | Move |
| **ESC** | Exit |

**Novo:** Keys 1-9 za group purchase! ???

---

**Slede?e:** Phase 8 — People Spawning & Animation ???????
