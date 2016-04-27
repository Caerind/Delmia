#include "ParticleComponent.hpp"
#include "World.hpp"

NParticleComponent::NParticleComponent()
: mSystemId("")
, mEmissionRate(0.f)
, mEmissionDifference(0.f)
, mParticleLifetime(sf::seconds(1.f))
, mParticlePosition(sf::Vector2f(0.f,0.f))
, mParticleVelocity(sf::Vector2f(0.f,0.f))
, mParticleRotation(0.f)
, mParticleRotationSpeed(0.f)
, mParticleScale(sf::Vector2f(1.f,1.f))
, mParticleColor(sf::Color::White)
, mParticleTextureIndex(0)
{
}

NParticleComponent::~NParticleComponent()
{
}

void NParticleComponent::connectToSystem(std::string const& systemId)
{
    mSystemId = systemId;
}

void NParticleComponent::disconnect()
{
    mSystemId = "";
}

void NParticleComponent::tick(sf::Time dt)
{
    emitParticles(computeParticleCount(dt));
}

void NParticleComponent::setEmissionRate(float particlesPerSecond)
{
    mEmissionRate = particlesPerSecond;
}

void NParticleComponent::setParticleLifetime(sf::Time lifetime)
{
    mParticleLifetime = lifetime;
}

void NParticleComponent::setParticlePosition(sf::Vector2f position)
{
    mParticlePosition = position;
}

void NParticleComponent::setParticleVelocity(sf::Vector2f velocity)
{
    mParticleVelocity = velocity;
}

void NParticleComponent::setParticleRotation(float rotation)
{
    mParticleRotation = rotation;
}

void NParticleComponent::setParticleRotationSpeed(float rotationSpeed)
{
    mParticleRotationSpeed = rotationSpeed;
}

void NParticleComponent::setParticleScale(sf::Vector2f scale)
{
    mParticleScale = scale;
}

void NParticleComponent::setParticleColor(sf::Color color)
{
    mParticleColor = color;
}

void NParticleComponent::setParticleTextureIndex(std::size_t textureIndex)
{
    mParticleTextureIndex = textureIndex;
}

void NParticleComponent::emitParticles(std::size_t particleAmount)
{
    NParticleSystem::Ptr system = NWorld::getParticleSystem(mSystemId);

    for (std::size_t i = 0; i < particleAmount; ++i)
	{
		NParticleSystem::Particle particle(mParticleLifetime);
		particle.position = mParticlePosition + NVector::NToSFML2F(getFinalPosition());
		particle.velocity = mParticleVelocity;
		particle.rotation = mParticleRotation;
		particle.rotationSpeed = mParticleRotationSpeed;
		particle.scale = mParticleScale;
		particle.color = mParticleColor;
		particle.textureIndex = mParticleTextureIndex;

        if (system != nullptr)
        {
            system->emitParticle(particle);
        }
	}
}

std::size_t NParticleComponent::computeParticleCount(sf::Time dt)
{
    // We want to fulfill the desired particle rate as exact as possible. Since the amount of emitted particles per frame is
	// integral, we have to emit sometimes more and sometimes less. mParticleDifference takes care of the deviation each frame.
	float particleAmount = mEmissionRate * dt.asSeconds() + mEmissionDifference;
	std::size_t nbParticles = static_cast<std::size_t>(particleAmount);

	// Compute difference for next frame, return current amount
	mEmissionDifference = particleAmount - nbParticles;
	return nbParticles;
}
