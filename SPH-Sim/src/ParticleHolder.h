#pragma once
#include <raylib.h>

#include <vector>
#include <raymath.h>

#define SMOOTHING_RADIUS 200.f
#define	STIFFNESS 20.0f
#define AIMED_DENSITY 100.0f

struct Particle {
	Vector2 Position;
	Vector2 Velocity;
	float Density, Mass;
};

// Used for calculating densities
static float Poly6Kernel(float distance, float smoothingRadius) {
	float value = std::max(0.f, smoothingRadius * smoothingRadius - distance * distance);
	float factor = 4.f / (3.14159265359f * powf(smoothingRadius, 8));
	return factor * value * value * value;
}

static float SpikyKernel(float distance, float smoothingRadius) { 
	float spiky = 15.0f / (3.14159265359f * powf(smoothingRadius, 6));
	spiky *= powf(smoothingRadius - distance, 3);
	return std::max(0.0f, spiky);
}

static Vector2 SpikyGradient(Vector2 pos_i, Vector2 pos_j, float smoothingRadius) {
    float distance = Vector2Length(pos_i - pos_j);
    if (distance <= 0.f || distance >= smoothingRadius)
		return { 0.0f, 0.0f };

	float coeff = -45.0f / (3.14159265359f * powf(smoothingRadius, 6));
    float dW = -3 * coeff * powf(smoothingRadius - distance, 2);
    Vector2 direction = Vector2Normalize(pos_i - pos_j);
    return direction * dW;
}

class ParticleHolder {
public:
	ParticleHolder();

	void Update(float deltaTime);
	void Render();

private:
	Vector2 GradientOfP(Particle& particle);

private:
	std::vector<Particle> m_Particles;

	uint32_t m_ParticleAmount = 200;
};
