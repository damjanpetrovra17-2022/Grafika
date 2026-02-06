# ?? Phase 4 — Quick Test Guide

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
Phong shader created with ID: 4  ? MORA biti
Scene initialized with hall geometry
[INFO] Application initialized successfully
```

### 4. Vizuelna Provera

**Odmah po pokretanju trebalo bi da vidiš:**

```
     PLAFON (svetao, gradijent sen?enja)
    ???????????????????????????
    ?                         ?
    ?   ?? Room Light         ? ? (0, 4.5, 0)
    ?   (toplo belo)          ?
    ?                         ?
    ?   ZIDOVI                ? ? Sen?enje vidljivo!
    ?   (gradijenti)          ?   (ne flat kao Phase 3)
    ?                         ?
    ?   POD (siv)             ? ? Svetliji u centru
    ?   ????????????          ?   (blizu svetla)
    ?                         ?
    ?   EKRAN (BEL!)          ? ? Bez uticaja svetla
    ?   ????????              ?   (unlit shader)
    ???????????????????????????
```

? **AKO VIDIŠ:**
- Sala **nije flat** (ima sen?enje/gradijente)
- Pod je **svetliji u centru** (blizu svetla)
- Zidovi imaju **dubinu** (ne jednobojno)
- **Specular highlights** na površinama
- Ekran ostaje **?isto beo**

? **RADI! Phase 4 je OK!** ?

? **AKO VIDIŠ:**
- Flat sen?enje (kao Phase 3) ? Phong shader ne radi
- Crnu salu ? Room light nije enabled
- Ekran pod uticajem svetla ? Dual-shader bug

---

## ?? Brzi Lighting Test

### Test 1: Default (Room Light ON)
**O?ekivano:**
- Sala osvetljena ?
- Sen?enje vidljivo ?
- Specular highlights ?

---

### Test 2: Room Light OFF
**U `Application.cpp`, dodaj nakon scene init:**
```cpp
m_scene->getRoomLight().enabled = false;
```

**Rebuild i pokreni:**
- Sala skoro crna (samo 10% ambient) ?
- Jako tamno ?

---

### Test 3: Screen Light ON
**U `Application.cpp`:**
```cpp
m_scene->getRoomLight().enabled = false;
m_scene->getScreenLight().enabled = true;
```

**Rebuild i pokreni:**
- Oblast ekrana osvetljena ?
- Zadnji deo sale taman ?
- Gradijent vidljiv ?

---

### Test 4: Kretanje
1. Kreni ka centru plafona ? Svetlije
2. Kreni ka uglovima ? Tamnije
3. Gledaj pod iz razli?itih uglova ? Specular highlights se pomeraju

? **Dinami?ko osvetljenje radi!**

---

## ? Success Criteria

- [x] Build uspešan
- [x] Phong shader kreiran (ID > 0)
- [x] Sala ima sen?enje (ne flat)
- [x] Pod svetliji u centru
- [x] Zidovi imaju dubinu
- [x] Specular highlights vidljivi
- [x] Ekran ostaje beo (unlit)
- [x] Room light OFF ? sala tamna
- [x] Screen light radi
- [x] Phase 0-3 features rade

**Ako je SVE ? ? Phase 4 DONE!**

---

## ?? Ako Nešto Ne Radi

### Problem: Sala flat (nema sen?enja)
```powershell
# Proveri da li Phong shader kompajliran:
# Konzola treba da kaže:
Vertex shader compiled successfully
Fragment shader compiled successfully
Phong shader created with ID: 4
```

**Rešenje:** Rebuild clean
```powershell
cmake --build build --config Debug --clean-first
```

---

### Problem: Sala crna
```cpp
// Proveri da je room light enabled:
// U Scene::createLights():
m_roomLight = Light(..., true);  // ? true = enabled
```

---

### Problem: Ekran pod uticajem svetla (nije beo)
```cpp
// Proveri da screen ima isScreen flag:
// U Scene::createHallGeometry():
m_objects.push_back(SceneObject(..., true));  // ? true = isScreen
```

---

**Slede?e:** Phase 5 — Seat System ??
