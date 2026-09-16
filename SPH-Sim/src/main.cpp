#include <raylib.h>

#include "ParticleHolder.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "SPH Simulation");

    SetTargetFPS(60);

	ParticleHolder particleHolder;

    // Main game loop
    while (!WindowShouldClose())
    {
		particleHolder.Update(0.0001f);

        BeginDrawing();
        ClearBackground(WHITE);

		particleHolder.Render();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
