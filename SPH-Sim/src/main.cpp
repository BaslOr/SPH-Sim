#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "raygui.h"
//#undef _CRT_SECURE_NO_WARNINGS

#include "ParticleHolder.h"

void DrawDebugUI(SimulationParams* simParams);

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "SPH Simulation");

    SetTargetFPS(60);

    SimulationParams simParams{};
	simParams.ParticleAmount = 200;

	simParams.SmoothingRadius = 200.0f;
    simParams.Stiffness = 100.0f;
    simParams.AimedDensity = 50.0f;
    simParams.ParticleMass = 10.0f;

	simParams.TimeStep = 0.00001f;

	ParticleHolder particleHolder(simParams);

    // Main game loop
    while (!WindowShouldClose())
    {
        particleHolder.UpdateParams(simParams);
		particleHolder.Update();

        BeginDrawing();
        ClearBackground(WHITE);


		particleHolder.Render();

        DrawDebugUI(&simParams);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// Programmed by chatgpt I take no responsiblity for this ;)
// ImGui will be implemented soon
void DrawDebugUI(SimulationParams* simParams)
{
    // ---------------------------------------------------------
    // Window position
    // ---------------------------------------------------------

    static Vector2 windowPos = { 15.0f, 15.0f };

    const float windowWidth = 370.0f;
    const float windowHeight = 340.0f;

    Rectangle window = {
        windowPos.x,
        windowPos.y,
        windowWidth,
        windowHeight
    };

    // ---------------------------------------------------------
    // Draggable header
    // ---------------------------------------------------------

    Rectangle header = {
        window.x,
        window.y,
        window.width,
        70.0f
    };

    static bool dragging = false;
    static Vector2 dragOffset = { 0.0f, 0.0f };

    Vector2 mouse = GetMousePosition();

    // Start dragging
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse, header))
    {
        dragging = true;

        dragOffset.x = mouse.x - window.x;
        dragOffset.y = mouse.y - window.y;
    }

    // Stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        dragging = false;
    }

    // Move window
    if (dragging)
    {
        windowPos.x = mouse.x - dragOffset.x;
        windowPos.y = mouse.y - dragOffset.y;

        // Keep window inside the screen
        if (windowPos.x < 0)
            windowPos.x = 0;

        if (windowPos.y < 0)
            windowPos.y = 0;

        if (windowPos.x + windowWidth > GetScreenWidth())
            windowPos.x = GetScreenWidth() - windowWidth;

        if (windowPos.y + windowHeight > GetScreenHeight())
            windowPos.y = GetScreenHeight() - windowHeight;

        window.x = windowPos.x;
        window.y = windowPos.y;
    }

    // ---------------------------------------------------------
    // Panel shadow
    // ---------------------------------------------------------

    DrawRectangleRounded(
        {
            window.x + 5.0f,
            window.y + 5.0f,
            window.width,
            window.height
        },
        0.08f,
        8,
        Fade(BLACK, 0.20f)
    );

    // ---------------------------------------------------------
    // Panel background
    // ---------------------------------------------------------

    DrawRectangleRounded(
        window,
        0.08f,
        8,
        Fade(BLACK, 0.85f)
    );

    // ---------------------------------------------------------
    // Header
    // ---------------------------------------------------------

    DrawText(
        "SPH Simulation",
        (int)window.x + 20,
        (int)window.y + 15,
        22,
        RAYWHITE
    );

    DrawText(
        "Simulation Parameters",
        (int)window.x + 20,
        (int)window.y + 43,
        14,
        LIGHTGRAY
    );

    // Header separator
    DrawLine(
        (int)window.x + 20,
        (int)window.y + 70,
        (int)window.x + window.width - 20,
        (int)window.y + 70,
        Fade(WHITE, 0.25f)
    );

    // ---------------------------------------------------------
    // Layout
    // ---------------------------------------------------------

    const float labelX = window.x + 20.0f;
    const float sliderX = window.x + 20.0f;

    // Slider
    const float sliderW = 230.0f;

    // Value is now further to the right
    const float valueX = window.x + 275.0f;

    // Value is slightly lower
    const float valueYOffset = 19.0f;

    const float startY = window.y + 92.0f;
    const float gapY = 55.0f;

    float y = startY;

    // ---------------------------------------------------------
    // Aimed Density
    // ---------------------------------------------------------

    DrawText(
        "Aimed Density",
        (int)labelX,
        (int)y,
        14,
        RAYWHITE
    );

    GuiSlider(
        {
            sliderX,
            y + 20.0f,
            sliderW,
            12.0f
        },
        "10",
        "200",
        &simParams->AimedDensity,
        10.0f,
        200.0f
    );

    DrawText(
        TextFormat("%.2f", simParams->AimedDensity),
        (int)valueX,
        (int)y + valueYOffset,
        14,
        SKYBLUE
    );

    // ---------------------------------------------------------
    // Smoothing Radius
    // ---------------------------------------------------------

    y += gapY;

    DrawText(
        "Smoothing Radius",
        (int)labelX,
        (int)y,
        14,
        RAYWHITE
    );

    GuiSlider(
        {
            sliderX,
            y + 20.0f,
            sliderW,
            12.0f
        },
        "50",
        "300",
        &simParams->SmoothingRadius,
        50.0f,
        300.0f
    );

    DrawText(
        TextFormat("%.2f", simParams->SmoothingRadius),
        (int)valueX,
        (int)y + valueYOffset,
        14,
        SKYBLUE
    );

    // ---------------------------------------------------------
    // Stiffness
    // ---------------------------------------------------------

    y += gapY;

    DrawText(
        "Stiffness",
        (int)labelX,
        (int)y,
        14,
        RAYWHITE
    );

    GuiSlider(
        {
            sliderX,
            y + 20.0f,
            sliderW,
            12.0f
        },
        "10",
        "200",
        &simParams->Stiffness,
        10.0f,
        200.0f
    );

    DrawText(
        TextFormat("%.2f", simParams->Stiffness),
        (int)valueX,
        (int)y + valueYOffset,
        14,
        SKYBLUE
    );

    // ---------------------------------------------------------
    // Particle Mass
    // ---------------------------------------------------------

    y += gapY;

    DrawText(
        "Particle Mass",
        (int)labelX,
        (int)y,
        14,
        RAYWHITE
    );

    GuiSlider(
        {
            sliderX,
            y + 20.0f,
            sliderW,
            12.0f
        },
        "0.1",
        "50",
        &simParams->ParticleMass,
        0.1f,
        50.0f
    );

    DrawText(
        TextFormat("%.2f", simParams->ParticleMass),
        (int)valueX,
        (int)y + valueYOffset,
        14,
        SKYBLUE
    );
}
