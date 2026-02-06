# ??? BRZI TEST — Stadium Platform & Stairs

## ? Test (1 minut)

### Pokreni
```powershell
.\out\build\x64-Debug\Debug\kostur.exe
```

---

## ? Šta O?ekuješ

### Odmah Na Startu:
```
   EKRAN (prednji zid)
      ?
      ?
  ??  ?  ? Vrata (levi zid)
      ?
      ?  [Platform]
      ?  ???? | ????  ? Sedišta
      ?  ???? | ????
      ?       ?
      ?   Stepenice
```

---

## ?? Test Vizuelnih Elemenata

### Test 1: Platforma
1. **Pogledaj ispod sedišta**

**Rezultat:**
- ? **Tamno-braon platforma** ispod sedišta
- ? Sedišta **ne lebde** više u vazduhu

**PLATFORMA RADI!** ?

---

### Test 2: Stepenice
1. **Pomeri kameru** ka centru izme?u leve i desne sekcije
2. **Pogledaj dole**

**Rezultat:**
- ? **5 stepena** koji se penju od poda
- ? Svaki stepen malo **viši** od prethodnog
- ? Stepenice u **sredini** izme?u sekcija

**STEPENICE RADE!** ?

---

### Test 3: Vrata
1. **Idi ka levom zidu** (pritisni A)
2. **Pogledaj ka ekranu** (mouse)

**Rezultat:**
- ? **Vrata na levom zidu** (ne na zadnjem)
- ? Bliže ekranu (Z = -5)
- ? **Braon pravougaonik** na zidu

**VRATA POMERENA!** ?

---

## ?? Test Kretanja

### Test 4: Levi Hodnik
1. **Idi levo** (A key)
2. **Kreni ka sedištima**

**Rezultat:**
- ? **Prostor izme?u lev og zida i sedišta**
- ? Možeš pro?i pored sedišta

---

### Test 5: Desni Hodnik
1. **Idi desno** (D key)
2. **Kreni ka sedištima**

**Rezultat:**
- ? **Prostor izme?u desnog zida i sedišta**
- ? Simetri?no sa levim hodnikom

---

### Test 6: Centralni Prolaz (Stepenice)
1. **Idi ka centru** izme?u leve i desne sekcije
2. **Penj se po stepenicama** (W key)

**Rezultat:**
- ? **Možeš i?i uz stepenice** ka zadnjim redovima
- ? Stepenice vode ka višim redovima

---

## ?? Boje

**Trebalo bi da vidiš:**
- **Platforma:** Tamno braon (0.3, 0.25, 0.2)
- **Stepenice:** Malo tamniji braon (0.28, 0.24, 0.22)
- **Vrata:** Braon-ish (0.4, 0.25, 0.15)

---

## ? Success Checklist

- [x] Platforma ispod sedišta vidljiva
- [x] 5 stepena u sredini vidljivo
- [x] Vrata na levom zidu (ne na zadnjem)
- [x] Levi hodnik postoji
- [x] Desni hodnik postoji
- [x] Stepenice vode ka zadnjim redovima

**Ako je SVE ? ? Hall Geometry Update DONE!**

---

**Rezultat:** Bioskop sada izgleda mnogo realisti?nije sa platformom i stepenicama! ?????
