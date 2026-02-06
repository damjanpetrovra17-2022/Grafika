# ??? CURSOR & CROSSHAIR FIX — Complete

## ? Šta Je Popravljeno

**Problem:** Kursor je bio isklju?en (FPS mode) i ray picking nije radio kako treba.

**Rešenje:**
1. ? Dodat **cursor toggle** — Tab key prebacuje izme?u FPS i Picking mode
2. ? Dodat **crosshair** (X) u centru ekrana za precizno nišanjenje
3. ? Camera mouse look radi SAMO kada je kursor skriven (FPS mode)
4. ? Ray picking radi SAMO kada je kursor vidljiv (Picking mode)

---

## ?? Nove Kontrole

### Cursor Mode Toggle (Tab Key)

**Pritisni TAB** da prebacuješ izme?u 2 moda:

#### 1. **VISIBLE Cursor Mode** (Picking Mode) — Default na startu
- ? Kursor **vidljiv** na ekranu
- ? **Crosshair (+)** u centru ekrana (beo X)
- ? **Left Click** bira sedišta (Free ? Reserved)
- ? Mouse movement **NE** pomera kameru
- ? WASD/Strelice **rade** (kretanje)

**Svrha:** Klikanje na sedišta, precizno nišanjenje

---

#### 2. **HIDDEN Cursor Mode** (FPS Mode)
- ? Kursor **skriven** (zaklju?an u centru)
- ? **Crosshair** se ne prikazuje
- ? **Mouse movement** pomera kameru (look around)
- ? Left Click **NE** bira sedišta
- ? WASD/Strelice **rade** (kretanje)

**Svrha:** Slobodno gledanje oko sebe (FPS camera)

---

## ?? Crosshair Detalji

**Izgled:**
```
        ?
?????????????????
        ?
```

**Boja:** Bela (1.0, 1.0, 1.0)  
**Pozicija:** Ta?no u centru ekrana  
**Veli?ina:** 2% ekrana (NDC space)  
**Rendering:** Uvek on top (depth test disabled)

**Svrha:**
- Pokazuje gde ?e ray picking biti uperen
- Olakšava precizno biranje sedišta
- Vidljiv SAMO u Picking mode (cursor visible)

---

## ??? Kako Koristiti

### Za Biranje Sedišta (Picking Mode)

1. **Pritisni Tab** (ako nisi ve? u picking mode)
2. **Pozicioniraj crosshair (+)** na sedište koje želiš
3. **Klikni levi miš** — sedište se toggle-uje (Free ? Reserved)

**Konzola:**
```
Cursor mode: VISIBLE (seat picking enabled)
Seat [r,c] -> Reserved
```

---

### Za Gledanje Oko Sebe (FPS Mode)

1. **Pritisni Tab** da skriješ kursor
2. **Pomeri miš** — kamera se pomera (look around)
3. **WASD** — kretanje

**Konzola:**
```
Cursor mode: HIDDEN (FPS camera mode)
```

---

## ? Test

### Test 1: Cursor Toggle
```powershell
cmake --build build --config Debug
.\out\build\x64-Debug\Debug\kostur.exe
```

**Na startu:**
- ? Kursor **vidljiv**
- ? **Beli crosshair (+)** u centru ekrana
- ? Konzola: `Cursor mode: VISIBLE (seat picking enabled)`

**Pritisni Tab:**
- ? Kursor **nestaje**
- ? Crosshair **nestaje**
- ? Mouse **pomera kameru**
- ? Konzola: `Cursor mode: HIDDEN (FPS camera mode)`

**Pritisni Tab opet:**
- ? Kursor se **vra?a**
- ? Crosshair se **vra?a**

---

### Test 2: Picking sa Crosshair-om

1. **Proveri da je cursor visible** (Tab ako treba)
2. **Pomeri crosshair (+)** na zeleno sedište ??
3. **Klikni** levi miš

**Rezultat:**
- ? Sedište postaje **žuto** ??
- ? Konzola: `Seat [r,c] -> Reserved`

---

### Test 3: FPS Camera Mode

1. **Pritisni Tab** (hidden mode)
2. **Pomeri miš** levo-desno

**Rezultat:**
- ? Kamera se **rotira** oko sebe
- ? Look around radi kao FPS

---

## ?? Izmenjeni Fajlovi

### Novi Fajlovi (2)
1. ? `Header/Crosshair.h` — Crosshair class
2. ? `Source/Crosshair.cpp` — Crosshair rendering

### Izmenjeni Fajlovi (8)
1. ? `Header/Input.h` — Added toggleCursorMode(), isCursorVisible()
2. ? `Source/Input.cpp` — Cursor toggle logic
3. ? `Header/Application.h` — Crosshair member, handleCursorToggle()
4. ? `Source/Application.cpp` — Crosshair init/draw, cursor toggle handling
5. ? `Source/Camera.cpp` — Mouse look only in FPS mode
6. ? `CMakeLists.txt` — Added Crosshair files

---

## ?? Default Ponašanje

**Na startu aplikacije:**
- Cursor: **VISIBLE** (picking mode)
- Crosshair: **PRIKAZAN**
- Mouse look: **ISKLJU?EN**
- Seat picking: **UKLJU?EN**

**Razlog:** Phase 6 je fokusiran na seat picking, pa je to default.

---

## ?? Kontrole (Kompletan Pregled)

| Kontrola | Picking Mode | FPS Mode |
|----------|--------------|----------|
| **Tab** | Switch to FPS | Switch to Picking |
| **Mouse Move** | ? No camera | ? Look around |
| **Left Click** | ? Pick seat | ? No picking |
| **WASD / Arrows** | ? Move | ? Move |
| **ESC** | ? Exit | ? Exit |
| **Crosshair** | ? Visible | ? Hidden |
| **Cursor** | ? Visible | ? Hidden |

---

## ?? Troubleshooting

### Problem: Crosshair se ne vidi
**Proveri:**
```
Cursor mode: VISIBLE  ? Mora biti
```

**Rešenje:** Pritisni **Tab** da prebacuješ u visible mode

---

### Problem: Miš ne pomera kameru
**Proveri:**
```
Cursor mode: HIDDEN  ? Mora biti za FPS mode
```

**Rešenje:** Pritisni **Tab** da skriješ kursor

---

### Problem: Klikovi ne biraju sedišta
**Proveri:**
1. Cursor visible? (Tab)
2. Crosshair vidljiv?
3. Booking mode enabled? (trebalo bi default)

**Rešenje:** Pritisni **Tab** da prikaže kursor

---

## ?? Tehni?ki Detalji

### Crosshair Rendering

**Vertex Data:**
```cpp
// 2 linije (horizontal + vertical)
// NDC koordinate (centriran na 0,0)
float size = 0.02f;  // 2% ekrana

Horizontal: (-size, 0) ? (+size, 0)
Vertical:   (0, -size) ? (0, +size)
```

**Shader:** Basic shader (identity MVP matrices)  
**Depth Test:** Disabled (always on top)  
**Color:** White (1, 1, 1)

---

### Cursor Toggle

**Tab Key ? Edge-triggered:**
```cpp
if (Input::isKeyPressed(GLFW_KEY_TAB))
{
    Input::toggleCursorMode();
}
```

**GLFW Modes:**
- Visible: `GLFW_CURSOR_NORMAL`
- Hidden: `GLFW_CURSOR_DISABLED`

---

### Camera Integration

**Mouse look disabled u picking mode:**
```cpp
if (!Input::isCursorVisible())
{
    processMouseLook(deltaX, deltaY);
}
```

**Keyboard movement uvek enabled.**

---

## ? Success Criteria

- [x] Cursor toggle radi (Tab)
- [x] Crosshair se prikazuje u picking mode
- [x] Crosshair je u centru ekrana
- [x] Mouse look radi samo u FPS mode
- [x] Seat picking radi u picking mode
- [x] WASD movement radi u oba moda
- [x] Build uspešan

**SVE DONE! ?**

---

## ?? Rezultat

**Sada imaš:**
- ? **Dual mode** — Picking i FPS (Tab toggle)
- ? **Crosshair** za precizno nišanjenje
- ? **Pravilno** ray picking (kursor vidljiv, clicking radi)
- ? **FPS camera** kada treba (kursor skriven)

**Best of both worlds!** ?????

---

**Status:** ? **FIXED**  
**Build:** ? **Successful**  
**Ready for:** Full Phase 6 Testing

---

**Dodato:** Crosshair + Cursor Toggle  
**Vreme:** ~30-45 minuta  
**LOC:** ~150 novih linija
