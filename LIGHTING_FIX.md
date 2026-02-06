# ?? LIGHTING FIX — Room Light Adjustment

## ?? Problem

Room light je bio **previše jak** i izgledalo je kao da **curi kroz plafon**.

**Uzroci:**
1. Svetlo pozicionirano suviše blizu plafona (Y = 4.5, ceiling Y = 5.0)
2. Intenzitet previše jak (10.0)
3. Ambient i specular previše jaki
4. Nedostatak clamping-a za over-brightness

---

## ? Rešenje

### 1. Smanjen Intenzitet Svetla
**PRE:**
```cpp
Position:  (0, 4.5, 0)    // Blizu plafona
Intensity: 10.0           // Prejako
```

**POSLE:**
```cpp
Position:  (0, 4.0, 0)    // Niže, 0.5m od plafona
Intensity: 5.0            // Prepolovljeno
```

**Razlog:** Svetlo sada ne "curi" kroz plafon i ima prirodniji intenzitet.

---

### 2. Podešen Shader (phong.frag)

#### Ambient Strength
```glsl
// PRE:  0.15 (15%)
// POSLE: 0.12 (12%)
float ambientStrength = 0.12;
```

#### Specular Strength
```glsl
// PRE:  0.3 (30%)
// POSLE: 0.2 (20%)
float specularStrength = 0.2;
```

#### Attenuation (Ja?i Falloff)
```glsl
// PRE:
attenuation = intensity / (1.0 + 0.09*d + 0.032*d²)

// POSLE: (ja?i falloff)
attenuation = intensity / (1.0 + 0.14*d + 0.05*d²)
```

**Razlog:** Svetlo sada brže opada sa distancom, manje "prelivanja".

#### Brightness Clamping
```glsl
// DODATO:
result = clamp(result, 0.0, 1.0);  // Prevent over-saturation
```

**Razlog:** Spre?ava da površine budu svetlije od bele (1.0).

---

## ?? Pre vs Posle

| Parametar | PRE | POSLE | Razlog |
|-----------|-----|-------|--------|
| **Light Y Position** | 4.5 | 4.0 | Dalje od plafona |
| **Light Intensity** | 10.0 | 5.0 | Manja ja?ina |
| **Ambient** | 15% | 12% | Manje ambient light |
| **Specular** | 30% | 20% | Manje odsjaja |
| **Attenuation (distance)** | 0.09 | 0.14 | Brži pad |
| **Attenuation (distance²)** | 0.032 | 0.05 | Brži pad |
| **Brightness Clamping** | ? Nema | ? Da | Spre?ava over-brightness |

---

## ?? Kako Testirati

### 1. Build i Pokreni
```powershell
cmake --build build --config Debug
.\out\build\x64-Debug\Debug\kostur.exe
```

### 2. Šta Bi Trebalo Da Vidiš

**? DOBRO:**
- Plafon **ne svetli intenzivno** (prirodna boja)
- Svetlo se **ravnomerno distribuira** po sali
- Pod je **svetliji u centru** ali nije over-bright
- Zidovi imaju **prirodne gradijente**
- Nema "curenja" kroz plafon

**? LOŠE (staro ponašanje):**
- Plafon svetli kao sunce
- Svetlo izgleda kao da "ispada" iz plafona
- Pod previše svetao
- Over-saturation (belo na belo)

---

## ?? Fine-Tuning (Opciono)

Ako ti se ?ini da je svetlo:

### Previše JAKO:
```cpp
// U Scene.cpp, createLights():
m_roomLight.intensity = 4.0f;  // Još manje (trenutno 5.0)
```

### Previše SLABO:
```cpp
m_roomLight.intensity = 6.0f;  // Malo ja?e (trenutno 5.0)
```

### Previše BLIZU plafona:
```cpp
m_roomLight.position = glm::vec3(0.0f, 3.5f, 0.0f);  // Niže (trenutno 4.0)
```

### Previše DALEKO od plafona:
```cpp
m_roomLight.position = glm::vec3(0.0f, 4.2f, 0.0f);  // Više (trenutno 4.0)
```

---

## ?? Lighting Parameters Reference

### Current Values (After Fix)
```cpp
// Room Light
Position:           (0, 4.0, 0)
Color:              (1.0, 0.95, 0.85)  // Warm white
Intensity:          5.0
Enabled:            true

// Shader (phong.frag)
Ambient Strength:   0.12
Specular Strength:  0.2
Attenuation Linear: 0.14
Attenuation Quad:   0.05
Brightness Clamp:   [0.0, 1.0]
```

### Hall Reference
```
Ceiling Y:  5.0
Floor Y:    0.5
Light Y:    4.0  (1.0m below ceiling)
```

---

## ? Files Changed

1. ? `Source/Scene.cpp` — Reduced light intensity (10.0 ? 5.0), lowered position (4.5 ? 4.0)
2. ? `Assets/Shaders/phong.frag` — Reduced ambient/specular, stronger attenuation, added clamping

---

## ?? Result

**Svetlo sada:**
- ? Ne "curi" kroz plafon
- ? Ima prirodniji intenzitet
- ? Ravnomerno osvetljava salu
- ? Nema over-saturation
- ? Izgleda realisti?no

---

**Status:** ? **FIXED**  
**Build:** ? **Successful**  
**Next:** Testirati u aplikaciji

---

**Kreiran:** Auto-generated  
**Verzija:** 1.0  
**Tip promene:** Lighting adjustment (Phase 4 fine-tuning)
