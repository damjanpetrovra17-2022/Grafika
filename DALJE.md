# DALJE.md — Kompletna Analiza Projekta i Specifikacija za Nastavak

## ?? TRENUTNO STANJE PROJEKTA

### Osnovne Informacije
- **Ime projekta:** Kostur (3D Cinema Simulator)
- **Executable:** kostur.exe
- **Jezik:** C++14
- **Build sistem:** CMake 3.31.6 (Ninja generator)
- **Grafi?ka API:** OpenGL 3.3 Core Profile
- **Biblioteke:** GLFW 3.4.0, GLEW 2.2.0, GLM (header-only), stb_image

---

## ? IMPLEMENTIRANE FAZE

### **Phase 0 — Project & Engine Setup** ? KOMPLETNO
**Status:** 100% implementirano

**Šta je ura?eno:**
- ? Organizovana struktura projekta: `Source/`, `Header/`, `Assets/`
- ? CMake build sistem konfigurisan
- ? Dependency management (NuGet paketi za GLEW/GLFW)
- ? GLM matemati?ka biblioteka integrirana
- ? stb_image za u?itavanje tekstura (header-only)

**Moduli:**
- `Application` — Glavni kontroler aplikacije (init/run/shutdown)
- `AppTime` (bivši `Time`) — Delta time tracking (`glfwGetTime()`)
- `Log` — Console logging sistem
- `Main.cpp` — Entry point

**Fajlovi:**
```
Source/Application.cpp
Source/AppTime.cpp
Source/Log.cpp
Source/Main.cpp
Header/Application.h
Header/AppTime.h
Header/Log.h
```

**Dokumentacija:**
- `third_party.md` — Lista dependency-ja
- `PHASE0_COMPLETE.md` — Completion report

---

### **Phase 1 — Window, Context, FPS Limiter, Fullscreen, Exit** ? KOMPLETNO
**Status:** 100% implementirano

**Šta je ura?eno:**
- ? **Window sistem** — Fullscreen prozor na primary monitoru
- ? **OpenGL 3.3 Core** context kreiranje i inicijalizacija (GLEW)
- ? **FrameLimiter** — 75 FPS cap sa target frame time 1/75
- ? **ESC exit** — Trenutni izlaz iz aplikacije (callback)
- ? **Event loop** — pollEvents() + swapBuffers()

**Moduli:**
- `Window` — GLFW prozor wrapper (fullscreen, context setup)
- `FrameLimiter` — FPS ograni?enje sa delta time merenje
- `Util` — Helper funkcije

**Fajlovi:**
```
Source/Window.cpp
Source/FrameLimiter.cpp
Source/Util.cpp
Header/Window.h
Header/FrameLimiter.h
Header/Util.h
```

**Dokumentacija:**
- `PHASE1_COMPLETE.md` — Completion report

---

### **Phase 2 — Camera (Perspective, Mouse Look, Movement, Collision)** ? KOMPLETNO
**Status:** 100% implementirano i verifikovano

**Šta je ura?eno:**
- ? **Input sistem** — Key down/pressed (edge detection), mouse delta tracking
- ? **First-person Camera** — Yaw/pitch rotacija, view-direction movement
- ? **Mouse look** — Smooth rotacija sa pitch clamping (±89°)
- ? **AABB Collision** — Kamera ne može da pro?e kroz zidove/pod/plafon
- ? **View-direction movement** — Strelice/WASD kre?u kameru u pravcu gledanja (NE fiksne ose)
- ? **Debug rendering** — Crvena kocka, pod, 4 corner markera za testiranje

**Moduli:**
- `Input` — GLFW input wrapper (tastatura + miš)
- `Camera` — FPS kamera sa GLM matricama
- `AABB` — Axis-aligned bounding box (collision)
- `DebugCube` — Jednostavan cube mesh (36 vertices)
- `Shader` — Shader wrapper klasa (basic.vert/basic.frag)

**Fajlovi:**
```
Source/Input.cpp
Source/Camera.cpp
Source/DebugCube.cpp
Header/Input.h
Header/Camera.h
Header/AABB.h
Header/DebugCube.h
Shader.cpp / Shader.h
Assets/Shaders/basic.vert
Assets/Shaders/basic.frag
```

**Kontrole:**
- Miš ? Look around (yaw/pitch, clamped)
- Arrow Up / W ? Napred (u smeru kamere)
- Arrow Down / S ? Nazad (u smeru kamere)
- Arrow Left / A ? Strafe levo (perpendikularno na pogled)
- Arrow Right / D ? Strafe desno (perpendikularno na pogled)
- ESC ? Izlaz

**Coordinate System:**
```
+Y (gore)
 ?
 ?
 ???? +X (desno)
/
+Z (nazad, ka vratima)

Ekran je na -Z (prednji zid)
Kamera spawn: (0, 1.7, 8) gledaju?i ka -Z
```

**Hall Bounds:**
- X: -9.0 do +9.0 (18m širina)
- Y: 0.5 do 5.0 (4.5m visina)
- Z: -9.0 do +9.0 (18m dubina)
- Padding: 0.3m od svih zidova

**Dokumentacija:**
- `PHASE2_COMPLETE.md` — Implementation summary
- `PHASE2_AUDIT_REPORT.md` — Detaljni verification report (5000+ re?i)
- `PHASE2_IMPLEMENTATION_SUMMARY.md` — Quick overview
- `PHASE2_VISUAL_TEST_GUIDE.md` — Manuelni testni scenariji

---

## ? FAZE KOJE TREBA IMPLEMENTIRATI (Phase 3-13)

### **Phase 3 — Basic Scene (Floor, Walls, Ceiling, Screen)** ? KOMPLETNO
**Status:** 100% implementirano

**Šta je ura?eno:**
- ? **Scene modul** — Kreiran `Scene.h` i `Scene.cpp`
- ? **Hall geometrija** — Pod, plafon, 4 zida renderovani
- ? **Ekran** — Beli quad (8×3m) na prednjem zidu
- ? **Vrata** — Braon placeholder (2×2.5m) na zadnjem zidu
- ? **Coordinate system** — Dokumentovano u `README_scene.md`
- ? **Camera collision** — AABB bounds poklapaju sa hall geometrijom

**Moduli:**
- `Scene` — Drži i renderuje scene objects (floor, walls, ceiling, screen, door)
- `SceneObject` — Jednostavan struct (position, scale, color)

**Fajlovi:**
```
Source/Scene.cpp
Header/Scene.h
README_scene.md
PHASE3_COMPLETE.md
PHASE3_SUMMARY.md
PHASE3_QUICK_TEST.md
```

**Dokumentacija:**
- `README_scene.md` — Coordinate system i hall layout
- `PHASE3_COMPLETE.md` — Completion report sa test uputstvima
- `PHASE3_SUMMARY.md` — Kratak pregled implementacije
- `PHASE3_QUICK_TEST.md` — 2-minutni test guide

---

### **Phase 4 — Lighting System (Room Light + Screen Light)** ? KOMPLETNO
**Status:** 100% implementirano

**Šta je ura?eno:**
- ? **Light struktura** — Kreiran `Light.h` (position/color/intensity/enabled)
- ? **Room Light** — Toplo svetlo blizu plafona, enabled by default
- ? **Screen Light** — Belo svetlo kod ekrana, disabled by default
- ? **Phong shader** — Implementiran Blinn-Phong lighting model
- ? **Mesh normale** — DebugCube extended sa normalama (6 floats/vertex)
- ? **Material colors** — Per-object uBaseColor support
- ? **Dual-shader** — Phong za salu, basic za ekran (unlit)

**Moduli:**
- `Light` — Struct za svetlosne podatke
- Phong shader — `phong.vert` + `phong.frag` (Blinn-Phong)
- Scene — Ažuriran za lighting rendering

**Fajlovi:**
```
Header/Light.h
Assets/Shaders/phong.vert
Assets/Shaders/phong.frag
Source/DebugCube.cpp (normale dodane)
Source/Scene.cpp (lighting support)
PHASE4_COMPLETE.md
PHASE4_SUMMARY.md
```

**Dokumentacija:**
- `PHASE4_COMPLETE.md` — Detaljni completion report
- `PHASE4_SUMMARY.md` — Kratak pregled (srpski)

---

### **Phase 5 — Seat System (3D Seats, Rows, Steps, States)** ? KOMPLETNO
**Status:** 100% implementirano

**Šta je ura?eno:**
- ? **Seat struktura** — Kreiran `Seat.h` (row/col/state/position/bounds)
- ? **SeatState enum** — Free, Reserved, Purchased
- ? **SeatGrid klasa** — 5 redova × 10 kolona (50 sedišta)
- ? **Stadium seating** — Svaki red postepeno viši (realisti?an bioskop)
- ? **Row elevation** — 0.3m po redu (ukupno 1.2m razlika)
- ? **AABB bounds** — Izra?unati za svako sedište (za Phase 6 picking)
- ? **State-based coloring** — Zeleno/Žuto/Crveno
- ? **Phong lighting** — Sedišta osvetljena kao i sala

**Moduli:**
- `Seat` — Struct za sedište (pozicija, state, bounds)
- `SeatGrid` — Grid sa stadium seating logikom

**Fajlovi:**
```
Header/Seat.h
Header/SeatGrid.h
Source/SeatGrid.cpp
PHASE5_COMPLETE.md
PHASE5_SUMMARY.md
```

**Stadium Seating Formula:**
```cpp
position.y = origin.y + row * rowElevationStep;
// rowElevationStep = 0.3m
// Row 0: Y=1.0m, Row 4: Y=2.2m (1.2m rise)
```

**Dokumentacija:**
- `PHASE5_COMPLETE.md` — Detaljni completion report
- `PHASE5_SUMMARY.md` — Kratak pregled (srpski)

---

### **Phase 6 — Seat Interaction (Mouse Ray Picking, Reserve/Unreserve)** ? SLEDE?A FAZA
**Status:** 0% implementirano

**Cilj:** Kreirati 3D grid sedišta (5 redova x 10 kolona) sa step-ovima i state-ovima

**Šta treba dodati:**

#### 1. SeatState Enum
```cpp
enum class SeatState {
    Free,      // Zeleno (ili default boja)
    Reserved,  // Žuto
    Purchased  // Crveno
};
```

#### 2. Seat Struktura
```cpp
struct Seat {
    int row;
    int col;
    SeatState state;
    glm::vec3 position;  // Centar sedišta u world space
    AABB bounds;         // Za ray picking (Phase 6)
};
```

#### 3. SeatGrid Klasa
```cpp
class SeatGrid {
public:
    static constexpr int ROWS = 5;
    static constexpr int COLS = 10;
    
    void init();
    void draw(Shader& shader);
    
    Seat* getSeat(int row, int col);
    
private:
    Seat seats[ROWS][COLS];
    float stepHeight = 0.3f;  // Svaki red je 0.3m viši
    float seatSpacingX = 1.0f;
    float seatSpacingZ = 1.2f;
};
```

**Pozicioniranje:**
- Red 0 (najbliži ekranu): Y = baseY
- Red 1: Y = baseY + stepHeight
- Red 2: Y = baseY + 2 * stepHeight
- ...
- Red 4 (najdalji od ekrana): Y = baseY + 4 * stepHeight

**Rendering:**
- Svako sedište = scaled cuboid (stolica)
- Boja prema state-u (Free=green, Reserved=yellow, Purchased=red)
- Teksture stolica later (Phase 13 polishing)

**Acceptance Criteria:**
- ? 50 sedišta vidljivo u sali
- ? Row stepping je o?igledan (zadnji redovi viši)
- ? Svako sedište ima stabilnu world poziciju i bounds

**Fajlovi koje treba kreirati:**
```
Source/SeatGrid.cpp
Header/SeatGrid.h
Seat.h (ve? postoji header stub)
```

---

### **Phase 6 — Seat Interaction (Mouse Ray Picking, Reserve/Unreserve)** ? NAKON PHASE 5
**Status:** 0% implementirano

**Cilj:** Klikanje na sedišta sa ray casting-om (Reserve ? Free)

**Šta treba dodati:**

#### 1. RayPicker Klasa
```cpp
class RayPicker {
public:
    Ray screenToWorldRay(
        double mouseX, double mouseY,
        int screenWidth, int screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection
    );
    
    bool rayAABBIntersection(const Ray& ray, const AABB& box, float& t);
    
    Seat* pickSeat(const Ray& ray, SeatGrid& grid);
};
```

#### 2. Mouse Click Interakcija
**Input handler (levi klik):**
```cpp
if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
    Ray ray = rayPicker.screenToWorldRay(...);
    Seat* seat = rayPicker.pickSeat(ray, seatGrid);
    
    if (seat) {
        if (seat->state == SeatState::Free) {
            seat->state = SeatState::Reserved;
        } else if (seat->state == SeatState::Reserved) {
            seat->state = SeatState::Free;
        }
        // Purchased seats: do nothing
    }
}
```

**Booking Mode Boolean:**
- Privremeni boolean `bool isBookingMode = true;` (dok ne do?e state machine u Phase 10)
- Interakcija dozvoljenja samo kada je `isBookingMode == true`

**Acceptance Criteria:**
- ? Klik na sedište ? Free ? Reserved toggle
- ? Purchased sedišta ne mogu biti toggleovana
- ? Picking bira najbliže sedište ako se više preklapaju u ray direction

**Fajlovi koje treba kreirati:**
```
Source/RayPicker.cpp
Header/RayPicker.h
Header/Ray.h (struct Ray { glm::vec3 origin, direction; })
```

---

### **Phase 7 — Purchase Logic (1-9 Keys, Adjacent Seat Search)** ? NAKON PHASE 6
**Status:** 0% implementirano

**Cilj:** Kupovina N susednih Free sedišta (1-9 tastere) sa ta?nim scan algoritmom

**Šta treba dodati:**

#### 1. SeatGrid::purchaseAdjacent(int N)
**Algoritam (TA?AN SCAN ORDER):**
```cpp
bool SeatGrid::purchaseAdjacent(int N) {
    // Scan od zadnjeg reda ka prvom
    for (int row = ROWS - 1; row >= 0; --row) {
        // Scan od desne kolone ka levoj
        for (int col = COLS - 1; col >= 0; --col) {
            // Proveri da li postoji blok od ta?no N uzastopnih Free sedišta
            // po?evši od (row, col) idu?i ulevo
            if (canFitBlock(row, col, N)) {
                // Markuj N sedišta kao Purchased
                for (int i = 0; i < N; ++i) {
                    seats[row][col - i].state = SeatState::Purchased;
                }
                return true;
            }
        }
    }
    return false;  // Nema dovoljno susednih Free sedišta
}
```

**Važno:**
- Samo **Free** sedišta se uzimaju u obzir (Reserved NE ra?unaju)
- Scan ide **od zadnjeg reda ka prvom**
- U redu: **od desne kolone ka levoj**
- Prvi prona?eni blok se kupuje

#### 2. Key Bindings (1-9)
```cpp
for (int i = 1; i <= 9; ++i) {
    if (Input::isKeyPressed(GLFW_KEY_1 + (i - 1))) {
        if (isBookingMode) {
            bool success = seatGrid.purchaseAdjacent(i);
            if (!success) {
                LOG_INFO("Ne postoji blok od " << i << " susednih sedišta!");
            }
        }
    }
}
```

**Acceptance Criteria:**
- ? Scan order odgovara 2D logici (desno ka levo, zadnji red ka prvom)
- ? Samo Free sedišta se kupuju
- ? Reserved sedišta NE ra?unaju kao Free za kupovinu

**Fajlovi za izmenu:**
```
Source/SeatGrid.cpp (dodati purchaseAdjacent metodu)
Source/Application.cpp (dodati key handlers)
```

---

### **Phase 8 — People System (Humanoid Models, Paths, Movement)** ? NAKON PHASE 7
**Status:** 0% implementirano

**Cilj:** Spawn ljudi na vratima i animacija do sedišta (staged path sa step climbing-om)

**Šta treba dodati:**

#### 1. Person i PeopleManager
```cpp
enum class MovementStage {
    MovingToAisle,   // 1. Kre?e se duž prolaza ka target redu
    ClimbingSteps,   // 2. Penje se na visinu target reda
    MovingToSeat,    // 3. Kre?e se popreko reda do sedišta
    Seated           // 4. Sedi (završio movement)
};

class Person {
public:
    glm::vec3 position;
    Seat* targetSeat;
    MovementStage stage;
    int modelIndex;  // 0-14 (za 15 razli?itih modela)
    
    void update(float deltaTime);
};

class PeopleManager {
public:
    void spawnPeople(int count, SeatGrid& grid);
    void updateAll(float deltaTime);
    void drawAll(Shader& shader);
    
private:
    std::vector<std::unique_ptr<Person>> people;
};
```

#### 2. Movement Path (Entry)
**4 stage-a:**
1. **MovingToAisle:** Menja Z ka target row Z
2. **ClimbingSteps:** Menja Y ka target row Y (step climbing)
3. **MovingToSeat:** Menja X ka target seat X (popreko reda)
4. **Seated:** Zaustavlja se

**Brzina:** Konstantna brzina bazirana na deltaTime (npr. 2.0 m/s)

#### 3. Target Seat Assignment
**Pravila:**
- Assign only Reserved OR Purchased seats
- Avoid duplicate assignments (jedan person = jedno sedište)
- Spawn random broj ljudi od 1 do occupiedCount

#### 4. Rendering
**Opcija A (lakše):** Renderuj ljude kao colored cubes (fallback)
**Opcija B (teže):** U?itaj 15 humanoid OBJ modela iz `Assets/Models/`

**Model loading:**
- Ako je teško, stavi iza compile-time ili runtime flaga
- Cube fallback mora raditi

**Acceptance Criteria:**
- ? Ljudi se kre?u staged (aisle ? climb ? across row ? seated)
- ? Ljudi dostižu target sedišta i zaustavljaju se
- ? Nema duplikata (2 ljudi ? isto sedište)
- ? Cube fallback radi ako modeli fale

**Fajlovi koje treba kreirati:**
```
Source/Person.cpp
Source/PeopleManager.cpp
Source/ModelLoader.cpp (opciono, za OBJ modele)
Header/Person.h
Header/PeopleManager.h
Header/ModelLoader.h
```

---

### **Phase 9 — Projection Simulation (Screen Textures, Timing, Film Loop)** ? NAKON PHASE 8
**Status:** 0% implementirano

**Cilj:** Film playback na ekranu (20 tekstura, 20 sekundi, reset na belo)

**Šta treba dodati:**

#### 1. Screen Component
```cpp
class Screen {
public:
    void init();
    void startPlayback();
    void update(float deltaTime);
    void draw(Shader& shader);
    
    bool isPlaying() const { return playing; }
    
private:
    std::vector<GLuint> filmTextures;  // 20 tekstura
    GLuint whiteTexture;
    
    bool playing;
    float timer;
    int currentFrame;
    
    static constexpr float FILM_DURATION = 20.0f;
    static constexpr int FRAME_COUNT = 20;
};
```

#### 2. Film Playback Logic
**Teksture:**
- U?itaj `Assets/Textures/film_frame_00.png` do `film_frame_19.png`
- Ukupno 20 frejmova

**Timing:**
- 20 sekundi ukupno
- 20 frejmova => 1 sekunda po frejmu
- `currentFrame = (int)(timer / (FILM_DURATION / FRAME_COUNT))`

**Nakon 20 sekundi:**
- `playing = false`
- Reset na white texture

#### 3. Screen Light Integration
**Iz Phase 4:**
```cpp
if (screen.isPlaying()) {
    lights.screenLight.enabled = true;
} else {
    lights.screenLight.enabled = false;
}
```

**Acceptance Criteria:**
- ? Ekran prikazuje prave teksture (ne random solid colors)
- ? Ukupno trajanje filma je 20 sekundi
- ? Ekran se resetuje na belo kada se završi

**Fajlovi koje treba kreirati:**
```
Source/Screen.cpp
Header/Screen.h
Assets/Textures/film_frame_00.png - film_frame_19.png (20 tekstura)
Assets/Textures/white.png
```

---

### **Phase 10 — State Machine (Booking ? Entry ? Projection ? Exit ? Reset)** ? NAKON PHASE 9
**Status:** 0% implementirano

**Cilj:** State machine kontroliše ceo ciklus (booking, ulazak, projekcija, izlazak, reset)

**Šta treba dodati:**

#### 1. AppState Enum
```cpp
enum class AppState {
    Booking,     // Interaktivni booking (reserve/purchase)
    Entering,    // Vrata otvorena, ljudi se kre?u ka sediš tima
    Projection,  // Vrata zatvorena, room light OFF, screen ON, film igra
    Exiting      // Vrata otvorena, ljudi se vra?aju ka vratima
};
```

#### 2. StateController (ili unutar Application)
**Transitions:**

1. **Booking ? Entering:**
   - Trigger: `Enter` taster pritisnuto
   - Uslov: Postoji bar 1 Reserved ili Purchased sedište
   - Akcija: Spawn random broj ljudi (1 do occupiedCount), otvori vrata

2. **Entering ? Projection:**
   - Trigger: Svi ljudi reported seated
   - Akcija: Zatvori vrata, ugasi room light, upali screen light, startuj film

3. **Projection ? Exiting:**
   - Trigger: Film se završio (20s elapsed)
   - Akcija: Otvori vrata, upali room light, ugasi screen light, komanduj ljude da se vrate

4. **Exiting ? Booking:**
   - Trigger: Svi ljudi exited (vratili se ka vratima)
   - Akcija: Clear people list, resetuj sva sedišta na Free

#### 3. Door Component
```cpp
class Door {
public:
    void open();
    void close();
    void draw(Shader& shader);
    
    bool isOpen() const { return opened; }
    
private:
    bool opened;
    glm::vec3 position;
    // Color/state change (animacija opciona)
};
```

#### 4. Lighting by State
| State | Room Light | Screen Light |
|-------|-----------|-------------|
| Booking | ON | OFF |
| Entering | ON | OFF |
| Projection | OFF | ON |
| Exiting | ON | OFF |

#### 5. Input Restrictions
- Reserve/Purchase dozvoljeno **samo u Booking** state-u
- U svim drugim state-ovima: input disabled

**Acceptance Criteria:**
- ? Full loop: Booking ? Entering ? Projection ? Exiting ? Booking
- ? Room light OFF tokom projekcije; screen light ON tokom projekcije
- ? Nakon exita: sedišta resetovana na Free, booking radi ponovo

**Fajlovi koje treba kreirati:**
```
Source/Door.cpp
Header/Door.h
Header/AppState.h (enum)
```

**Izmene:**
```
Source/Application.cpp (dodati state machine logiku)
```

---

### **Phase 11 — Rendering Features (Depth Test Toggle, Back-Face Culling Toggle)** ? NAKON PHASE 10
**Status:** 0% implementirano

**Cilj:** Runtime toggle za depth testing i back-face culling

**Šta treba dodati:**

#### 1. Key Toggles
```cpp
// D taster ? toggle depth test
if (Input::isKeyPressed(GLFW_KEY_D)) {
    depthTestEnabled = !depthTestEnabled;
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    LOG_INFO("Depth Test: " << (depthTestEnabled ? "ON" : "OFF"));
}

// C taster ? toggle back-face culling
if (Input::isKeyPressed(GLFW_KEY_C)) {
    cullingEnabled = !cullingEnabled;
    if (cullingEnabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
    LOG_INFO("Back-Face Culling: " << (cullingEnabled ? "ON" : "OFF"));
}
```

#### 2. Validacija
**VAŽNO:**
- Kada je depth test ON, toggling culling **NE SME** da proizvede vidljivu razliku
- Ovo zna?i da su mesh-evi zatvoren i imaju consistent winding

**Test:**
1. Uklju?i depth test (D)
2. Toggle culling (C) nekoliko puta
3. Proveri da se ništa ne menja vizuelno

**Acceptance Criteria:**
- ? Oba toggle-a rade runtime u svim state-ovima
- ? Sa depth test ON, culling toggle ne proizvodi vidljivu razliku

**Fajlovi za izmenu:**
```
Source/Application.cpp (dodati key handlers i state tracking)
```

---

### **Phase 12 — HUD Overlay (Student Name/Index, Transparency)** ? NAKON PHASE 11
**Status:** 0% implementirano

**Cilj:** Semi-transparent HUD nametag u uglu ekrana (ortho projekcija)

**Šta treba dodati:**

#### 1. HUD Klasa
```cpp
class HUD {
public:
    void init();
    void draw();  // Ortho projection, ne koristi 3D view
    
private:
    GLuint nametagTexture;
    GLuint quadVAO, quadVBO;
    Shader hudShader;
};
```

#### 2. Rendering u Screen Space
**Koraci:**
1. Disable depth testing za HUD draw
2. Koristiti orthographic projection (NDC coords)
3. Renderuj textured quad u uglu (npr. bottom-right)
4. Enable blending za alpha transparency
5. Draw HUD **last** (nakon 3D scene)

**HUD Shader:**
```glsl
// hud.vert
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);  // NDC space
    TexCoord = aTexCoord;
}

// hud.frag
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D hudTexture;

void main() {
    FragColor = texture(hudTexture, TexCoord);
}
```

**Blending:**
```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// Draw HUD
glDisable(GL_BLEND);
```

**Acceptance Criteria:**
- ? HUD tekstura je semi-transparent (alpha) i ?itljiva
- ? HUD pozicija je fiksna na ekranu dok se kamera pomera
- ? Vidljiv u svim app state-ovima, uklju?uju?i Projection

**Fajlovi koje treba kreirati:**
```
Source/HUD.cpp
Header/HUD.h
Assets/Shaders/hud.vert
Assets/Shaders/hud.frag
Assets/Textures/hud_nametag.png (ime, prezime, indeks sa alpha kanalom)
```

---

### **Phase 13 — Polishing & Validation (Edge Cases, Testing)** ? POSLEDNJA FAZA
**Status:** 0% implementirano

**Cilj:** Hardening aplikacije, validacija, debug UI, testiranje

**Šta treba dodati:**

#### 1. Validation Checks
**Provere:**
- ? ESC izlazi trenutno u SVAKOM state-u
- ? Fullscreen ostaje aktivan
- ? Frame limiter ostaje 75 FPS
- ? Seat interactions disabled izvan Booking state-a
- ? Enter u Booking sa 0 occupied seats ? ništa se ne dešava
- ? People spawning nikad ne prekora?uje occupied seat count
- ? Nema duplikata target seats
- ? Film uvek traje 20s i resetuje ekran na belo
- ? Nakon Exiting: sedišta reset na Free, people list prazan

#### 2. Debug UI (Console logs)
**Print:**
- Current app state (Booking/Entering/Projection/Exiting)
- Occupied seat count
- People count
- Depth test status (ON/OFF)
- Culling status (ON/OFF)

**Primer:**
```
[DEBUG] State: Booking | Seats: 12 Occupied | People: 0 | DepthTest: ON | Culling: ON
```

#### 3. Testing Checklist
**Kreirati `TESTING.md`:**
- Checklist za svaku fazu (Phase 0-12)
- End-to-end scenario (pun ciklus Booking ? Projection ? Reset)
- Manual test steps

**Acceptance Criteria:**
- ? Nema regresija: sve prethodne faze rade
- ? End-to-end run je ponovljiv bez restarta
- ? Debug output jasno pokazuje trenutni state i brojeve

**Fajlovi koje treba kreirati:**
```
TESTING.md
```

**Fajlovi za izmenu:**
```
Source/Application.cpp (dodati validation checks i debug prints)
Source/SeatGrid.cpp (dodati edge case handling)
Source/PeopleManager.cpp (provere duplikata)
```

---

## ?? STRUKTURA FAJLOVA

### Trenutno Postoje?i Fajlovi

#### Source Files (Source/)
```
Application.cpp    — Glavni kontroler aplikacije
AppTime.cpp        — Delta time tracking
Camera.cpp         — First-person kamera
DebugCube.cpp      — Debug cube mesh
FrameLimiter.cpp   — 75 FPS cap
Input.cpp          — Keyboard + mouse input
Log.cpp            — Console logging
Main.cpp           — Entry point
Util.cpp           — Utility funkcije
Window.cpp         — GLFW window wrapper
```

#### Header Files (Header/)
```
AABB.h            — Axis-aligned bounding box
Application.h     — Application class
AppTime.h         — Time class
Camera.h          — Camera class
DebugCube.h       — DebugCube class
FrameLimiter.h    — FrameLimiter class
Input.h           — Input class
Log.h             — Logging macros
stb_image.h       — Image loader (third-party)
Util.h            — Utility funkcije
Window.h          — Window class
```

#### Root Files
```
Rectangle.cpp / Rectangle.h  — (Legacy, možda nije koriš?en)
Shader.cpp / Shader.h        — Shader wrapper
Seat.h                       — Seat struct (stub)
CMakeLists.txt               — Build konfiguracija
packages.config              — NuGet dependencies
```

#### Assets
```
Assets/Shaders/basic.vert    — Basic MVP vertex shader
Assets/Shaders/basic.frag    — Basic flat color fragment shader
```

#### Dokumentacija
```
CMAKE_BUILD_GUIDE.md                — CMake build uputstvo
PHASE0_COMPLETE.md                  — Phase 0 completion report
PHASE1_COMPLETE.md                  — Phase 1 completion report
PHASE2_AUDIT_REPORT.md              — Phase 2 detaljni audit
PHASE2_COMPLETE.md                  — Phase 2 summary
PHASE2_IMPLEMENTATION_SUMMARY.md    — Phase 2 overview
PHASE2_VISUAL_TEST_GUIDE.md         — Phase 2 test scenariji
prompts.md                          — Copilot prompts (sve faze)
QUICK_REFERENCE.md                  — Quick commands
README.md                           — Project overview
SOLUTION_SUMMARY.md                 — Solution summary
START_HERE.md                       — Quick start guide
Spec.md                             — Original specification
third_party.md                      — Dependency lista
```

---

### Fajlovi Koje Treba Kreirati (Phase 3-13)

#### Phase 3 — Basic Scene
```
Source/Scene.cpp
Source/SceneObject.cpp
Header/Scene.h
Header/SceneObject.h
README_scene.md
```

#### Phase 4 — Lighting System
```
Assets/Shaders/phong.vert
Assets/Shaders/phong.frag
Header/Light.h
Source/LightingManager.cpp (opciono)
```

#### Phase 5 — Seat System
```
Source/SeatGrid.cpp
Header/SeatGrid.h
(Seat.h ve? postoji kao stub)
```

#### Phase 6 — Seat Interaction
```
Source/RayPicker.cpp
Header/RayPicker.h
Header/Ray.h
```

#### Phase 7 — Purchase Logic
(Izmene postoje?ih fajlova)

#### Phase 8 — People System
```
Source/Person.cpp
Source/PeopleManager.cpp
Source/ModelLoader.cpp (opciono)
Header/Person.h
Header/PeopleManager.h
Header/ModelLoader.h (opciono)
Assets/Models/person_00.obj - person_14.obj (15 modela)
```

#### Phase 9 — Projection Simulation
```
Source/Screen.cpp
Header/Screen.h
Assets/Textures/film_frame_00.png - film_frame_19.png
Assets/Textures/white.png
```

#### Phase 10 — State Machine
```
Source/Door.cpp
Header/Door.h
Header/AppState.h
```

#### Phase 11 — Rendering Features
(Izmene postoje?ih fajlova)

#### Phase 12 — HUD Overlay
```
Source/HUD.cpp
Header/HUD.h
Assets/Shaders/hud.vert
Assets/Shaders/hud.frag
Assets/Textures/hud_nametag.png
```

#### Phase 13 — Polishing & Validation
```
TESTING.md
```

---

## ?? BUILD SISTEM

### CMake Konfiguracija
```cmake
cmake_minimum_required(VERSION 3.16)
project(KosturProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable: kostur.exe
add_executable(kostur ${SOURCE_FILES} ${HEADER_FILES})

# Include dirs:
# - glm/ (math library)
# - packages/glew.../include (OpenGL extension loader)
# - packages/glfw.../include (windowing library)
# - Header/ (project headers)

# Link libraries:
# - OpenGL::GL
# - glew32s.lib (static)
# - glfw3.lib (static)
```

### Build Commands
```bash
# Configure
cmake -B build -G Ninja

# Build (Debug)
cmake --build build --config Debug

# Build (Release)
cmake --build build --config Release

# Output
out/build/x64-Debug/Debug/kostur.exe
out/build/x64-Release/Release/kostur.exe
```

### Dependencies (NuGet)
```xml
<package id="glew-2.2.0" version="2.2.0.1" targetFramework="native" />
<package id="glfw" version="3.4.0" targetFramework="native" />
```

**Restore:**
```powershell
.\nuget.exe restore packages.config -PackagesDirectory packages
```

---

## ?? PRIORITETI ZA NASTAVAK

### 1. **Phase 3 — Basic Scene** (SLEDE?E)
**Procena vremena:** 4-6 sati

**Koraci:**
1. Kreirati `Scene` i `SceneObject` klase
2. Dodati geometriju (pod, zidovi, plafon, vrata)
3. Dodati ekran quad (beo)
4. Dokumentovati coordinate system u `README_scene.md`
5. Testirati walkthrough u sali

**Prioritet:** ????? (KRITI?AN — foundation za sve ostalo)

---

### 2. **Phase 4 — Lighting System**
**Procena vremena:** 3-5 sati

**Koraci:**
1. Implementirati Phong shader
2. Dodati normale u mesh data
3. Kreirati 2 svetla (room + screen)
4. Testirati osvetljenje

**Prioritet:** ???? (VISOK — uti?e na vizuelni kvalitet)

---

### 3. **Phase 5 — Seat System**
**Procena vremena:** 4-6 sati

**Koraci:**
1. Implementirati `SeatGrid` klasu
2. Kreirati 50 sedišta (5x10 grid)
3. Dodati step-ove (row height increase)
4. Renderovati sa bojama prema state-u

**Prioritet:** ????? (KRITI?AN — core feature)

---

### 4. **Phase 6 — Seat Interaction (Ray Picking)**
**Procena vremena:** 5-7 sati

**Koraci:**
1. Implementirati `RayPicker` (screen to world ray)
2. Ray-AABB intersection test
3. Mouse click handler (Reserve ? Free)
4. Testirati picking accuracy

**Prioritet:** ????? (KRITI?AN — user interaction)

---

### 5. **Phase 7 — Purchase Logic**
**Procena vremena:** 2-3 sati

**Koraci:**
1. Implementirati `purchaseAdjacent(N)` sa ta?nim scan algoritmom
2. Dodati 1-9 key bindings
3. Testirati edge cases (malo sedišta, fragmentacija)

**Prioritet:** ???? (VISOK — core booking logic)

---

### 6. **Phase 8 — People System**
**Procena vremena:** 8-12 sati (sa modelima), 4-6 sati (bez modela)

**Koraci:**
1. Implementirati `Person` i `PeopleManager`
2. Staged movement (aisle ? climb ? seat)
3. Target seat assignment (bez duplikata)
4. (Opciono) Model loading (15 OBJ modela)

**Prioritet:** ????? (KRITI?AN — animation core)

---

### 7. **Phase 9 — Projection Simulation**
**Procena vremena:** 3-5 sati

**Koraci:**
1. Implementirati `Screen` klasu
2. U?itati 20 film frame tekstura
3. Timing logic (1s/frame, 20s total)
4. Screen light integration

**Prioritet:** ???? (VISOK — vizuelni feature)

---

### 8. **Phase 10 — State Machine**
**Procena vremena:** 6-8 sati

**Koraci:**
1. Implementirati `AppState` enum
2. State transitions (Booking ? Entering ? Projection ? Exiting)
3. Door component (open/close)
4. Lighting control po state-u
5. Input restrictions

**Prioritet:** ????? (KRITI?AN — application flow)

---

### 9. **Phase 11 — Rendering Features (Toggles)**
**Procena vremena:** 1-2 sati

**Koraci:**
1. D key ? depth test toggle
2. C key ? back-face culling toggle
3. Validacija (culling ne uti?e sa depth ON)

**Prioritet:** ?? (NIZAK — demonstracioni feature)

---

### 10. **Phase 12 — HUD Overlay**
**Procena vremena:** 2-3 sati

**Koraci:**
1. Implementirati ortho rendering
2. Kreirati HUD shader
3. U?itati nametag teksturu (sa alpha)
4. Blending setup

**Prioritet:** ??? (SREDNJI — nice-to-have)

---

### 11. **Phase 13 — Polishing & Validation**
**Procena vremena:** 4-6 sati

**Koraci:**
1. Dodati validation checks
2. Debug UI (console logs)
3. Edge case handling
4. Kreirati `TESTING.md`
5. End-to-end testiranje

**Prioritet:** ????? (KRITI?AN — quality assurance)

---

## ?? PROCENA UKUPNOG VREMENA

| Faza | Procena (sati) | Prioritet | Status |
|------|---------------|-----------|--------|
| Phase 0 | — | ????? | ? DONE |
| Phase 1 | — | ????? | ? DONE |
| Phase 2 | — | ????? | ? DONE |
| Phase 3 | 4-6 ? 1-2 | ????? | ? DONE |
| **Phase 4** | **3-5 ? 3-4** | ???? | ? **DONE** |
| Phase 5 | 4-6 | ????? | ? TODO |
| Phase 6 | 5-7 | ????? | ? TODO |
| Phase 7 | 2-3 | ???? | ? TODO |
| Phase 8 | 8-12 | ????? | ? TODO |
| Phase 9 | 3-5 | ???? | ? TODO |
| Phase 10 | 6-8 | ????? | ? TODO |
| Phase 11 | 1-2 | ?? | ? TODO |
| Phase 12 | 2-3 | ??? | ? TODO |
| Phase 13 | 4-6 | ????? | ? TODO |
| **UKUPNO** | **37-58** | — | **38% DONE** |

---

## ?? PREPORUKA ZA NASTAVAK

### Slede?i Koraci (Ordered by Priority)

#### 1. **IMPLEMENTIRAJ PHASE 3 — Basic Scene**
**Razlog:** Foundation za sve renderovanje, mora biti prvo.

**Akcija:**
```bash
# Koristi prompts iz prompts.md Phase 3
# Ili direktno implementiraj Scene modul
```

**Deliverables:**
- Scene rendering sa podom, zidovima, plafonom
- Ekran quad (beo)
- Vrata placeholder
- `README_scene.md` dokumentacija

---

#### 2. **IMPLEMENTIRAJ PHASE 4 — Lighting**
**Razlog:** Osvetljenje zna?ajno poboljšava vizuelni kvalitet.

**Akcija:**
- Implementiraj Phong shader
- Dodaj room light i screen light

---

#### 3. **IMPLEMENTIRAJ PHASE 5 ? 7 (Seat System)**
**Razlog:** Core booking functionality.

**Akcija:**
- Phase 5: SeatGrid rendering
- Phase 6: Ray picking
- Phase 7: Purchase logic

---

#### 4. **IMPLEMENTIRAJ PHASE 8 ? 10 (Animation + State Machine)**
**Razlog:** Core application flow.

**Akcija:**
- Phase 8: People movement
- Phase 9: Film playback
- Phase 10: State machine

---

#### 5. **IMPLEMENTIRAJ PHASE 11 ? 13 (Polish)**
**Razlog:** Finishing touches.

**Akcija:**
- Phase 11: Rendering toggles
- Phase 12: HUD
- Phase 13: Validation i testing

---

## ?? KAKO KORISTITI OVAJ DOKUMENT

### Za Implementaciju Nove Faze

1. **Otvori `prompts.md`** i na?i odgovaraju?u fazu
2. **Pro?itaj cilj i acceptance criteria**
3. **Pogledaj "Šta treba dodati" sekciju u ovom dokumentu**
4. **Implementiraj fajlove**
5. **Testiraj prema acceptance criteria**
6. **Kreiraj `PHASE_X_COMPLETE.md` dokument**

---

### Za Proveru Trenutnog Stanja

1. **Pogledaj "IMPLEMENTIRANE FAZE" sekciju**
2. **Proveri koje moduli postoje** u "STRUKTURA FAJLOVA"
3. **Build i pokreni** aplikaciju:
   ```bash
   cmake --build build --config Debug
   ./out/build/x64-Debug/Debug/kostur.exe
   ```

---

### Za Planiranje Vremena

1. **Pogledaj "PROCENA UKUPNOG VREMENA" tabelu**
2. **Prioritizuj prema prioritetu** (????? = kriti?no)
3. **Planiraj 4-6 sati za Phase 3** (slede?e)

---

## ?? DODATNI RESURSI

### Dokumentacija Projekta
- `prompts.md` — Copilot prompts za sve faze
- `PHASE2_VISUAL_TEST_GUIDE.md` — Test scenariji za fazu 2
- `CMAKE_BUILD_GUIDE.md` — Build uputstvo

### External Resources
- [GLFW Documentation](https://www.glfw.org/docs/latest/)
- [GLEW Documentation](http://glew.sourceforge.net/)
- [GLM Documentation](https://github.com/g-truc/glm)
- [LearnOpenGL](https://learnopengl.com/) — Phong lighting tutorial
- [Ray-AABB Intersection](https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection)

---

## ? CHECKLIST ZA SLEDE?U SESIJU

Pre nego što po?neš sa Phase 3:

- [ ] Pro?itaj `prompts.md` Phase 3 sekciju
- [ ] Pro?itaj "Phase 3" sekciju u ovom dokumentu
- [ ] Proveri da Phase 2 radi (pokreni aplikaciju, testiraj kameru)
- [ ] Napravi backup projekta (git commit ili zip)
- [ ] Pripremi reference za coordinate system (Y up, Z back, X right)
- [ ] Kreiraj branch (opciono): `git checkout -b phase-3-scene`

---

## ?? KRAJNJI CILJ

**Kompletan 3D Cinema Simulator sa:**
- ? First-person kamera (DONE)
- ? 3D bioskopska sala (TODO Phase 3)
- ? Interaktivni booking sedišta (TODO Phase 5-7)
- ? Animacija ljudi (TODO Phase 8)
- ? Film projekcija (TODO Phase 9)
- ? State machine flow (TODO Phase 10)
- ? Polish i validation (TODO Phase 11-13)

**Procenjeno vreme do završetka:** 40-60 sati rada

**Trenutni progres:** ~23% (3/13 faza)

---

**Sa?uvano:** `DALJE.md`  
**Datum kreiranja:** Auto-generated  
**Verzija:** 1.0  
**Status:** ? Kompletna analiza spremna za nastavak
