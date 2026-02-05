#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "../Header/Util.h"
#include "../Shader.h"
#include "../Rectangle.h"
#include "../Header/stb_image.h"
#include "../Seat.h"

GLFWwindow* window;

// ----------------------
// POMOĆNE KONSTANTE
// ----------------------
const float DOOR_X = -0.95f;
const float DOOR_Y = 0.8f;

// Usporenje kretanja ljudi – sve traje duže
const float PERSON_SPEED = 0.01f;   // korak po frejmu (manje = sporije)

// Faze aplikacije
enum AppPhase
{
    PHASE_BOOKING,   // rezervacije / kupovina
    PHASE_ENTERING,  // ljudi ulaze i sedaju
    PHASE_MOVIE,     // film traje
    PHASE_EXITING    // ljudi izlaze, reset stanja
};

// ----------------------
// KLASA/STRUKT ZA ČOVEKA
// ----------------------
struct Person
{
    Rectangle rect;
    Seat* targetSeat;
    int stage;

    Person(float startX, float startY, Seat* target, Shader* shader)
        : rect(startX, startY, 0.06f, 0.06f, shader),
        targetSeat(target),
        stage(0)
    {
        rect.setColor(0.2f, 1.0f, 0.2f, 1.0f);
    }
};

// Pretvaranje pixel u OpenGL koordinate
void getMouseGL(float& glX, float& glY)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    glX = (mx / (float)w) * 2.0f - 1.0f;
    glY = 1.0f - (my / (float)h) * 2.0f;
}

int main()
{
    std::cout << "Program start!" << std::endl;
    std::srand((unsigned)std::time(nullptr));

    if (!glfwInit())
    {
        std::cout << "GLFW init failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Kostur", NULL, NULL);
    if (!window)
    {
        std::cout << "Window create failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // =========================
    //   ENABLE VSYNC (75 Hz)
    // =========================
    glfwSwapInterval(1);


    // ======================================================
    // CUSTOM KURSOR – kamera
    // ======================================================
    int imgW, imgH, imgChannels;
    unsigned char* cursorPixels = stbi_load("camera.png", &imgW, &imgH, &imgChannels, 4);

    if (cursorPixels)
    {
        GLFWimage image;
        image.width = imgW;
        image.height = imgH;
        image.pixels = cursorPixels;

        GLFWcursor* cameraCursor = glfwCreateCursor(&image, imgW / 2, imgH / 2);
        glfwSetCursor(window, cameraCursor);
    }
    else
    {
        std::cout << "Cursor image loading failed!" << std::endl;
    }

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader rectShader("rectangle.vs", "rectangle.fs");

    // --- GRID sedišta ---
    std::vector<Seat> seats;

    int rows = 5, cols = 10;
    float startX = -0.8f, startY = 0.6f;
    float seatW = 0.12f, seatH = 0.12f;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            seats.emplace_back(startX + c * 0.15f, startY - r * 0.15f,
                seatW, seatH, &rectShader);

    // CENTRIRANO PLATNO
    Rectangle screen(-0.125f, 0.85f, 1.25f, 0.12f, &rectShader);
    screen.setColor(1.0f, 1.0f, 1.0f, 1.0f);

    Rectangle door(DOOR_X, DOOR_Y, 0.12f, 0.2f, &rectShader);
    door.setColor(0.6f, 0.4f, 0.2f, 1.0f);

    Rectangle overlay(0.0f, 0.0f, 2.0f, 2.0f, &rectShader);
    overlay.setColor(0.0f, 0.0f, 0.0f, 0.5f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // ======================================================
    // PERSONALNI TAG
    // ======================================================
    float tagW = 1.0f;       // širina
    float tagH = tagW / 2.0f; // visina (2:1 odnos)

    Rectangle nameTag(-0.60f, -0.80f, tagW, tagH, &rectShader);
    nameTag.setTexture("name.png");

    bool leftPressedLastFrame = false;
    bool keyPressedLastFrame = false;
    bool enterLastFrame = false;

    AppPhase phase = PHASE_BOOKING;

    std::vector<Person> people;

    double movieStartTime = 0.0;
    double lastColorChangeTime = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // ======================================================
        // PHASE_BOOKING
        // ======================================================
        if (phase == PHASE_BOOKING)
        {
            float glX, glY;
            getMouseGL(glX, glY);

            for (auto& seat : seats)
            {
                float dx = fabs(glX - seat.rect.x);
                float dy = fabs(glY - seat.rect.y);

                seat.isHovered =
                    (seat.state != Seat::PURCHASED) &&
                    dx < seat.rect.width / 2 &&
                    dy < seat.rect.height / 2;
            }

            bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

            if (leftPressed && !leftPressedLastFrame)
                for (auto& seat : seats)
                    if (seat.isHovered) { seat.toggleReserve(); break; }

            leftPressedLastFrame = leftPressed;

            bool anyNumberPressed =
                glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS;

            if (anyNumberPressed && !keyPressedLastFrame)
            {
                int N = 0;
                for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++)
                    if (glfwGetKey(window, key) == GLFW_PRESS)
                        N = key - GLFW_KEY_0;

                int found = 0;

                for (int i = seats.size() - 1; i >= 0; i--)
                {
                    if (seats[i].state == Seat::FREE)
                    {
                        found++;
                        if (found == N)
                        {
                            for (int j = 0; j < N; j++)
                                seats[i + j].buy();
                            break;
                        }
                    }
                    else found = 0;
                }
            }

            keyPressedLastFrame = anyNumberPressed;

            bool enterPressed = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;

            if (enterPressed && !enterLastFrame)
            {
                std::vector<Seat*> occ;
                for (auto& s : seats)
                    if (s.state == Seat::RESERVED || s.state == Seat::PURCHASED)
                        occ.push_back(&s);

                if (!occ.empty())
                {
                    int count = 1 + (rand() % occ.size());
                    people.clear();

                    for (int i = 0; i < count; i++)
                        people.emplace_back(DOOR_X, DOOR_Y,
                            occ[rand() % occ.size()],
                            &rectShader);

                    phase = PHASE_ENTERING;
                    overlay.setColor(0, 0, 0, 0);
                    door.setColor(0.8f, 0.4f, 0.0f, 1.0f);
                }
            }

            enterLastFrame = enterPressed;
        }
        else
        {
            leftPressedLastFrame = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            keyPressedLastFrame = false;
            enterLastFrame = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
        }

        // ======================================================
        // ANIMACIJA ULASKA / IZLASKA
        // ======================================================
        if (phase == PHASE_ENTERING || phase == PHASE_EXITING)
        {
            bool allSeated = true;
            bool allGone = true;

            for (auto& p : people)
            {
                float& px = p.rect.x;
                float& py = p.rect.y;
                float tx = p.targetSeat->rect.x;
                float ty = p.targetSeat->rect.y;

                if (phase == PHASE_ENTERING)
                {
                    if (p.stage == 0)
                    {
                        if (fabs(py - ty) > 0.001f) py += (ty > py ? 1 : -1) * PERSON_SPEED;
                        else p.stage = 1;
                    }
                    else if (p.stage == 1)
                    {
                        if (fabs(px - tx) > 0.001f) px += (tx > px ? 1 : -1) * PERSON_SPEED;
                        else p.stage = 2;
                    }

                    if (p.stage != 2) allSeated = false;
                }
                else if (phase == PHASE_EXITING)
                {
                    if (p.stage == 2) p.stage = 3;

                    if (p.stage == 3)
                    {
                        if (fabs(px - DOOR_X) > 0.001f)
                            px += (DOOR_X > px ? 1 : -1) * PERSON_SPEED;
                        else p.stage = 4;
                    }
                    else if (p.stage == 4)
                    {
                        if (fabs(py - DOOR_Y) > 0.001f)
                            py += (DOOR_Y > py ? 1 : -1) * PERSON_SPEED;
                        else p.stage = 5;
                    }

                    if (p.stage != 5) allGone = false;
                }
            }

            if (phase == PHASE_ENTERING && !people.empty() && allSeated)
            {
                phase = PHASE_MOVIE;
                door.setColor(0.6f, 0.4f, 0.2f, 1.0f);

                movieStartTime = glfwGetTime();
                lastColorChangeTime = movieStartTime;
            }

            if (phase == PHASE_EXITING && !people.empty() && allGone)
            {
                people.clear();
                for (auto& s : seats)
                    s.state = Seat::FREE;

                overlay.setColor(0, 0, 0, 0.5f);
                door.setColor(0.6f, 0.4f, 0.2f, 1.0f);

                screen.setColor(1, 1, 1, 1);
                phase = PHASE_BOOKING;
            }
        }

        // ======================================================
        // FILM SCENA
        // ======================================================
        if (phase == PHASE_MOVIE)
        {
            double now = glfwGetTime();
            double elapsed = now - movieStartTime;

            if (now - lastColorChangeTime >= 0.3)
            {
                lastColorChangeTime = now;
                float r = (rand() % 100) / 100.f;
                float g = (rand() % 100) / 100.f;
                float b = (rand() % 100) / 100.f;
                screen.setColor(r, g, b, 1);
            }

            if (elapsed >= 20.0)
            {
                screen.setColor(1, 1, 1, 1);
                door.setColor(0.2f, 1.0f, 0.2f, 1.0f);

                phase = PHASE_EXITING;
            }
        }

        // ======================================================
        // CRTANJE SCENE
        // ======================================================
        screen.draw();
        door.draw();

        for (auto& seat : seats)
            seat.draw();

        for (auto& p : people)
            p.rect.draw();

        if (phase == PHASE_BOOKING)
            overlay.draw();

        // --- CRTANJE PERSONALNOG TAGA ---
        nameTag.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
