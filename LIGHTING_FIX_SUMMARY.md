# ? BRZI FIX — Svetlo Podešeno

## ? Šta Je Popravljeno

**Problem:** Room light previše jako, "curi" kroz plafon

**Rešenje:**
1. ? Smanjen intenzitet: **10.0 ? 5.0**
2. ? Spušteno svetlo: **Y 4.5 ? 4.0** (1m od plafona)
3. ? Smanjen ambient: **15% ? 12%**
4. ? Smanjen specular: **30% ? 20%**
5. ? Ja?i attenuation (brži pad sa distancom)
6. ? Dodat brightness clamping (0.0 - 1.0)

---

## ?? Testiranje

```powershell
cmake --build build --config Debug
.\out\build\x64-Debug\Debug\kostur.exe
```

**Trebalo bi da vidiš:**
- ? Plafon prirodne boje (ne svetli intenzivno)
- ? Ravnomerno osvetljenje sale
- ? Nema "curenja" kroz plafon
- ? Pod svetliji u centru (normalno)

---

## ?? Ako Treba Dalje Podešavanje

### Previše Jako:
```cpp
// Scene.cpp, createLights():
m_roomLight.intensity = 4.0f;  // Smanji
```

### Previše Slabo:
```cpp
m_roomLight.intensity = 6.0f;  // Pove?aj
```

### Pomeraj Svetlo:
```cpp
// Niže (dalje od plafona):
m_roomLight.position = glm::vec3(0.0f, 3.5f, 0.0f);

// Više (bliže plafonu):
m_roomLight.position = glm::vec3(0.0f, 4.2f, 0.0f);
```

---

## ?? Nove Vrednosti

```cpp
Room Light:
- Position:  (0, 4.0, 0)    ? Bilo 4.5
- Intensity: 5.0            ? Bilo 10.0
- Ambient:   12%            ? Bilo 15%
- Specular:  20%            ? Bilo 30%
```

---

**Status:** ? Fixed  
**Build:** ? Successful  
**Detalji:** Vidi `LIGHTING_FIX.md`
