# ?? BRZI START - Testiranje Rendering Fix-a

## Koraci (5 minuta)

### 1?? Build Projekat
```powershell
cmake --build build --config Debug
```

**O?ekivano:**
- Build successful
- "Copying Assets folder to build directory" poruka

---

### 2?? Proveri da su Assets Kopirani
```powershell
Test-Path "out\build\x64-Debug\Debug\Assets\Shaders\basic.vert"
```

**O?ekivano:** `True`

---

### 3?? Pokreni Aplikaciju
```powershell
.\run_debug.ps1
```

**ALTERNATIVA:**
```powershell
.\out\build\x64-Debug\Debug\kostur.exe
```

---

### 4?? Proveri Konzolu

**Trebalo bi da vidiš:**
```
[INFO] Initializing Application...
[INFO] Creating fullscreen window...
[INFO] Window created successfully: 1920x1080
Vertex shader compiled successfully
Fragment shader compiled successfully
Shader program linked successfully
[INFO] Basic shader created with ID: 3
[INFO] OpenGL Vendor: ...
[INFO] OpenGL Renderer: ...
[INFO] Application initialized successfully
[INFO] Starting main loop...
```

**AKO VIDIŠ "ERROR:"** ? Kopiraj celu poruku i javi

---

### 5?? Proveri Vizuelni Output

**Trebalo bi da vidiš:**
- ? Tamno plavo-siva pozadina
- ? Crvena kocka na centru
- ? Sivi pod
- ? 4 obojene kocke na uglovima (zelena, plava, žuta, ljubi?asta)

**AKO VIDIŠ CRNI EKRAN** ? Idi na korak 6

---

### 6?? Testiraj Mouse Look
- Pomeri miš levo/desno
- Pomeri miš gore/dole

**O?ekivano:** Prikaz se rotira

**AKO NE RADI** ? Provjeri da li konzola pokazuje promenu kamere

---

### 7?? Testiraj Movement
- **W** ? Napred
- **S** ? Nazad  
- **A** ? Levo
- **D** ? Desno

**O?ekivano:** Kretanje u pravcu gledanja

**AKO NE RADI** ? Provjeri konzolni output za camera pos promene

---

### 8?? Testiraj ESC
- Pritisni **ESC**

**O?ekivano:** Aplikacija se zatvara trenutno

---

## ? Uspešan Test

Ako SVE radi:
- ? 3D scena vidljiva
- ? Mouse look radi
- ? Movement radi
- ? ESC exit radi

**? Phase 2 je KOMPLETNA! ?**

---

## ? Ako Ne Radi

### Scenario 1: "ERROR: Failed to open shader file"
**Rešenje:**
```powershell
# Rebuild
cmake --build build --config Debug --clean-first

# Proveri Assets
Get-ChildItem "out\build\x64-Debug\Debug\Assets"
```

---

### Scenario 2: Crni Ekran, Nema Grešaka u Konzoli
**Rešenje:**
1. Proveri OpenGL verziju u konzoli (mora biti 3.3+)
2. Update GPU drivere
3. Dodaj ovaj logging u `Application::run()`:
   ```cpp
   LOG_INFO("About to draw - Shader ID: " + std::to_string(m_basicShader->ID));
   ```

---

### Scenario 3: Mouse/Movement Ne Radi
**Rešenje:**
1. Proveri da li konzola prikazuje camera pos promenu
2. Dodaj logging u `Camera::update()`:
   ```cpp
   LOG_INFO("MouseDelta: " + std::to_string(mouseX) + ", " + std::to_string(mouseY));
   ```

---

## ?? Pomo?

Ako ništa ne radi:
1. Kopiraj **SVE** iz konzole
2. Napravi screenshot ekrana
3. Javi Problem + Output + Screenshot

---

## ?? Detaljnije Informacije

- `RENDERING_FIX_SUMMARY.md` — Kompletan opis promena
- `RENDERING_FIX_REPORT.md` — Tehni?ki diagnostic report
- `PHASE2_VISUAL_TEST_GUIDE.md` — Detaljni test scenariji

---

**Vreme testiranja:** ~5 minuta  
**Status:** ? Sve promene implementirane, ?eka testiranje
