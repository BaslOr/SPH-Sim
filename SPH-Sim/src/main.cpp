#include <raylib.h>


#include <rlImGui.h>
#include <imgui.h>

#include "ParticleHolder.h"

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
    
    rlImGuiSetup(true);

    // Main game loop
    while (!WindowShouldClose())
    {
        particleHolder.UpdateParams(simParams);
		particleHolder.Update();

        BeginDrawing();
        ClearBackground(WHITE);


		particleHolder.Render();

        //---------------------ImGui------------------------------------------
        rlImGuiBegin();
		ImGui::Begin("Simulation Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Simulation Infos");
        ImGui::Text("");
		ImGui::Text("Particle Amount: %d", simParams.ParticleAmount);
		ImGui::Text("FPS: %.2f", GetFPS());
        ImGui::Separator();

		ImGui::Text("Simulation Parameters");
		ImGui::Text("");
		ImGui::SliderFloat("Aimed Density", &simParams.AimedDensity, 10.0f, 200.0f);
		ImGui::SliderFloat("Smoothing Radius", &simParams.SmoothingRadius, 10.0f, 200.0f);
		ImGui::SliderFloat("Stiffness", &simParams.Stiffness, 10.0f, 200.0f);
		ImGui::SliderFloat("Particle Mass", &simParams.ParticleMass, 1.0f, 100.0f);
        ImGui::Separator();

        ImGui::End();
        rlImGuiEnd();
        //--------------------------------------------------------------------

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
