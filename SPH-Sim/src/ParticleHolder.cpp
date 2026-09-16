#include "ParticleHolder.h"

ParticleHolder::ParticleHolder()
{
	for (uint32_t i = 0; i < m_ParticleAmount; i++)
	{
		Particle particle;
		particle.Position = { static_cast<float>(GetRandomValue(0, 1280)), static_cast<float>(GetRandomValue(0, 720)) };
		particle.Velocity = { 0.0f, 0.0f };
		particle.Density = 1.0f;
		particle.Mass = 1.0f;
		m_Particles.push_back(particle);
	}
}

void ParticleHolder::Update(float deltaTime)
{
	for (auto& particle : m_Particles)
	{
		//Calculate density
		particle.Density = 0.0f;
		for (const auto& other : m_Particles)
		{
			float distance = sqrtf(powf(particle.Position.x - other.Position.x, 2) + powf(particle.Position.y - other.Position.y, 2));
			if (distance < SMOOTHING_RADIUS)
			{
				particle.Density += Poly6Kernel(distance, SMOOTHING_RADIUS) * other.Mass;
			}
		}

		//Calculate pressure
		float pressure_i = STIFFNESS * (particle.Density - AIMED_DENSITY);


		//Calculate Force
		Vector2 force = { 0.0f, 0.0f };
		Vector2 gradientOfP = GradientOfP(particle);
		force.x = -(1 / particle.Density) * gradientOfP.x;
		force.y = -(1 / particle.Density) * gradientOfP.y;

		//Calculate final position
		Vector2 acceleration = { force.x / particle.Density, force.y / particle.Density };
		particle.Velocity += acceleration * deltaTime;
		particle.Position += particle.Velocity * deltaTime;

		if (particle.Position.x < 0.0f || particle.Position.x > 1280.0f)
		{
			particle.Velocity.x *= -1.0f;
		}

		if (particle.Position.y < 0.0f || particle.Position.y > 720.0f)
		{
			particle.Velocity.y *= -1.0f;
		}
	}
}

void ParticleHolder::Render()
{
	for (const auto& particle : m_Particles)
	{
		DrawCircleV(particle.Position, 2.0f, RED);
		//DrawLine(particle.Position.x, particle.Position.y, particle.Position.x + particle.Velocity.x * 5.0f, particle.Position.y + particle.Velocity.y * 5.0f, BLUE);
	}
}

Vector2 ParticleHolder::GradientOfP(Particle& particle)
{
	Vector2 sum = { 0.0f, 0.0f };
	for (Particle& other : m_Particles)
	{
		//if (particle == other) continue;

		float distance = Vector2Length(particle.Position - other.Position);
		float thisPressure = STIFFNESS * (particle.Density - AIMED_DENSITY);
		float otherPressure = STIFFNESS * (other.Density - AIMED_DENSITY);
		Vector2 direction = Vector2Normalize(particle.Position - other.Position);
		sum += direction*  other.Mass * ((thisPressure / (powf(particle.Density, 2))) + (otherPressure / (powf(other.Density, 2)))) * SpikyGradient(particle.Position, other.Position, SMOOTHING_RADIUS);
	}

	sum *= particle.Density;

	return sum;
}
