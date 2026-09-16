#include "ParticleHolder.h"

ParticleHolder::ParticleHolder(const SimulationParams& params)
	: params(params)
{


	for (uint32_t i = 0; i < params.ParticleAmount; i++)
	{
		Particle particle;
		particle.Position = { static_cast<float>(GetRandomValue(0, 1280)), static_cast<float>(GetRandomValue(0, 720)) };
		particle.LastPosition = particle.Position;
		particle.Density = 1.0f;
		particle.Mass = 1.0f;

		m_Particles.push_back(particle);
	}
}

void ParticleHolder::Update()
{
	for (auto& particle : m_Particles)
	{
		//Calculate density
		particle.Density = 0.0f;
		for (const auto& other : m_Particles)
		{
			float distance = sqrtf(powf(particle.Position.x - other.Position.x, 2) + powf(particle.Position.y - other.Position.y, 2));
			if (distance < params.SmoothingRadius)
			{
				particle.Density += Poly6Kernel(distance, params.SmoothingRadius) * other.Mass;
			}
		}

		//Calculate pressure
		float pressure_i = params.Stiffness * (particle.Density - params.AimedDensity);


		//Calculate Force
		Vector2 force = { 0.0f, 0.0f };
		Vector2 gradientOfP = GradientOfP(particle);
		force.x = -(1 / particle.Density) * gradientOfP.x;
		force.y = -(1 / particle.Density) * gradientOfP.y;
		
		//force.y -= GRAVITY * particle.Mass;

		//Calculate final position, using Verlet Integration
		Vector2 acceleration = {
			force.x / particle.Density,
			force.y / particle.Density 
		};
		Vector2 newPosition =
			particle.Position * 2.0f
			- particle.LastPosition
			+ acceleration * powf(params.TimeStep, 2);

		particle.LastPosition = particle.Position;
		particle.Position = newPosition;

		//Check Bounds
		particle.Position.x = std::max(0.0f, std::min(1280.0f, particle.Position.x));
		particle.Position.y = std::max(0.0f, std::min(720.0f, particle.Position.y));
	}
}

void ParticleHolder::Render()
{
	for (const auto& particle : m_Particles)
	{
		DrawCircleV(particle.Position, params.SmoothingRadius, Fade(BLUE, 0.1f));
	}
	for (const auto& particle : m_Particles)
	{
		DrawCircleV(particle.Position, 2.0f, RED);
		float scalar = 0.0001f;
		Vector2 velocity = (particle.Position - particle.LastPosition) / params.TimeStep;
		DrawLine(particle.Position.x, particle.Position.y, particle.Position.x + velocity.x * scalar, particle.Position.y + velocity.y * scalar, GREEN);
	}
}

Vector2 ParticleHolder::GradientOfP(Particle& particle)
{
	Vector2 sum = { 0.0f, 0.0f };
	for (Particle& other : m_Particles)
	{
		//if (particle == other) continue;

		float distance = Vector2Length(particle.Position - other.Position);
		float thisPressure = params.Stiffness * (particle.Density - params.AimedDensity);
		float otherPressure = params.Stiffness * (other.Density - params.AimedDensity);
		Vector2 SpikyGrad = SpikyGradient(particle.Position, other.Position, params.SmoothingRadius);
		sum.x += other.Mass * ((thisPressure / (powf(particle.Density, 2))) + (otherPressure / (powf(other.Density, 2)))) * SpikyGrad.x;
		sum.y += other.Mass * ((thisPressure / (powf(particle.Density, 2))) + (otherPressure / (powf(other.Density, 2)))) * SpikyGrad.y;
	}

	sum *= particle.Density;

	return sum;
}
