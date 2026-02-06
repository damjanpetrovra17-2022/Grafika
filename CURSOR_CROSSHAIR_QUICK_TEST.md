# ?? BRZI TEST — Cursor & Crosshair

## ? Test (1 minut)

### 1. Pokreni
```powershell
.\out\build\x64-Debug\Debug\kostur.exe
```

---

## ? Šta O?ekuješ

### Odmah Na Startu:
```
???????????????????????????
?                         ?
?         ?               ?
?    ???????????  ? X     ?  Beli crosshair u centru
?         ?               ?
?                         ?
?    ??????????          ?  Sedišta
?                         ?
???????????????????????????

+ Kursor vidljiv (može se pomerati)
```

**Konzola:**
```
Crosshair initialized
Cursor mode: VISIBLE (seat picking enabled)
```

---

## ??? Test Klikanja

### 1. Pozicioniraj Crosshair
- **Pomeri miš** da centriraš **crosshair (+)** na zeleno sedište ??

### 2. Klikni
- **Levi klik miša**

**Rezultat:**
- ? Sedište postaje **žuto** ??
- ? Konzola: `Seat [r,c] -> Reserved`

**CROSSHAIR RADI!** ?

---

## ?? Test Cursor Toggle

### Pritisni **Tab**

**Rezultat:**
- ? Kursor **nestaje**
- ? Crosshair **nestaje**
- ? Mouse **pomera kameru** (look around)
- ? Konzola: `Cursor mode: HIDDEN (FPS camera mode)`

**FPS MODE RADI!** ?

---

### Pritisni **Tab** Opet

**Rezultat:**
- ? Kursor se **vra?a**
- ? Crosshair se **vra?a**
- ? Mouse **ne pomera kameru**
- ? Konzola: `Cursor mode: VISIBLE (seat picking enabled)`

**TOGGLE RADI!** ?

---

## ?? Kontrole

| Key | Action |
|-----|--------|
| **Tab** | Toggle Cursor (Picking ? FPS) |
| **Mouse** | Move cursor (Picking) / Look around (FPS) |
| **Left Click** | Pick seat (Picking only) |
| **WASD** | Move camera (Always) |
| **ESC** | Exit |

---

## ?? Dva Moda

### Picking Mode (Default)
```
Cursor:    ? Visible
Crosshair: ? Visible (+)
Clicking:  ? Pick seats
Mouse:     ? No camera rotation
```

### FPS Mode (Tab)
```
Cursor:    ? Hidden
Crosshair: ? Hidden
Clicking:  ? No picking
Mouse:     ? Camera rotation
```

---

**Ako SVE radi ? Crosshair Fix DONE! ?**

**Problem Fixed:** Cursor i crosshair sada rade kako treba! ??
