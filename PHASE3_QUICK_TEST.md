# ?? Phase 3 — Quick Test Guide

## ? Brzi Test (2 minuta)

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
[INFO] Scene initialized with hall geometry  ? MORA biti
[INFO] Application initialized successfully
```

### 4. Vizuelna Provera

**Odmah po pokretanju trebalo bi da vidiš:**

```
     PLAFON (svetlo siv)
    ???????????????????????
    ?                     ?
    ?    VRATA (braon)    ? ? Zadnji zid (iza tebe)
    ?                     ?
    ?                     ?
    ?   ?? TI SI OVDE     ? ? (0, 1.7, 8)
    ?   (gledaš dole ?)   ?
    ?                     ?
    ?   POD (siv)         ?
    ?                     ?
    ?   EKRAN (BEL!)      ? ? Prednji zid (ispred)
    ?   ????????          ?
    ???????????????????????
```

? **AKO VIDIŠ:**
- Sivi pod ispod
- Plafon iznad
- Zidove oko sebe
- **Veliki beli pravougaonik** (ekran) ispred
- Braon pravougaonik (vrata) iza

? **RADI! Phase 3 je OK!** ?

---

## ?? Brzi Movement Test

1. **W** ? Kreni ka ekranu (beli pravougaonik se približava)
2. **S** ? Kreni ka vratima (braon pravougaonik se približava)
3. **A/D** ? Kreni levo/desno duž sale
4. **Miš** ? Rotiraj pogled

---

## ?? Brzi Collision Test

1. Idi ka bilo kom zidu (drži W)
2. Pokušaj da pro?eš kroz zid
3. **O?ekivano:** Zaustaviš se ~0.3m od zida

? **Ako se zaustaviš** ? Collision radi!

---

## ? Ako Nešto Ne Radi

### Crni ekran?
```powershell
# Rebuild
cmake --build build --config Debug --clean-first
```

### Ekran nije bel?
- Proveri `Scene.cpp` liniju ~95: `screenColor(1.0f, 1.0f, 1.0f)`

### Mogu da pro?em kroz zid?
- Proveri `Application.cpp` AABB bounds: `(-9, 0.5, -9)` do `(9, 5, 9)`

---

## ? Success Criteria

- [x] Vidiš salu (pod + plafon + zidove)
- [x] Vidiš **beli ekran** (veliki pravougaonik ispred)
- [x] Vidiš vrata (braon pravougaonik iza)
- [x] Možeš da se kre?eš (WASD + miš)
- [x] Ne možeš da pro?eš kroz zidove
- [x] ESC zatvara aplikaciju

**Ako je SVE ? ? Phase 3 DONE!**

---

**Slede?e:** Phase 4 — Lighting System ??
