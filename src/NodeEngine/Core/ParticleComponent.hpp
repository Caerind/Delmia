#ifndef NPARTICLECOMPONENT_HPP
#define NPARTICLECOMPONENT_HPP

#include "Component.hpp"
#include "ParticleSystem.hpp"

class NParticleComponent : public NComponent
{
    public:
        NParticleComponent();
        ~NParticleComponent();

        void tick(sf::Time dt);

        void setEmissionRate(float particlesPerSecond);
        void setParticleLifetime(sf::Time lifetime);
        void setParticlePosition(sf::Vector2f position);
        void setParticleVelocity(sf::Vector2f velocity);
        void setParticleRotation(float rotation);
        void setParticleRotationSpeed(float rotationSpeed);
        void setParticleScale(sf::Vector2f scale);
        void setParticleColor(sf::Color color);
        void setParticleTextureIndex(std::size_t textureIndex);

        void emitParticles(std::size_t particleAmount);

        void connectToSystem(std::string const& systemId);
        void disconnect();

    private:
        std::size_t computeParticleCount(sf::Time dt);

    private:
        std::string mSystemId;

        float mEmissionRate;
        float mEmissionDifference;

        sf::Time mParticleLifetime;
        sf::Vector2f mParticlePosition;
        sf::Vector2f mParticleVelocity;
        float mParticleRotation;
        float mParticleRotationSpeed;
        sf::Vector2f mParticleScale;
        sf::Color mParticleColor;
        std::size_t mParticleTextureIndex;
};

#endif // NPARTICLECOMPONENT_HPP
