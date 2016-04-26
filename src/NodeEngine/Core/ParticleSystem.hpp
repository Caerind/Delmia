#ifndef NPARTICLESYSTEM_HPP
#define NPARTICLESYSTEM_HPP

#include "SceneComponent.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <array>
#include <functional>
#include <memory>

class NParticleSystem : public NSceneComponent
{
    public:
        typedef std::shared_ptr<NParticleSystem> Ptr;

        typedef std::array<sf::Vertex,4> Quad;

        class Particle
        {
            public:
                explicit Particle(sf::Time lifetime) : position(), velocity(), rotation(), rotationSpeed(), scale(1.f,1.f), color(255,255,255), textureIndex(0), passedLifetime(sf::Time::Zero), totalLifetime(lifetime) {}

                sf::Vector2f position;
                sf::Vector2f velocity;
                float rotation;
                float rotationSpeed;
                sf::Vector2f scale;
                sf::Color color;
                std::size_t textureIndex;

                sf::Time passedLifetime;
                sf::Time totalLifetime;
        };

        typedef std::function<void(Particle&, sf::Time)> Affector;

    public:
        NParticleSystem();

        void setTexture(std::string const& id);
        void setTexture(sf::Texture& texture);
        std::size_t addTextureRect(sf::IntRect const& rect);

        void addAffector(Affector affector);
        void clearAffectors();

        virtual void tick(sf::Time dt);
        virtual void render(sf::RenderTarget& target);

        std::size_t getParticleCount() const;
        void clearParticles();

        void emitParticle(Particle const& particle);

    private:
        void updateParticle(Particle& particle, sf::Time dt);
        void computeVertices();
        void computeQuads();
        void computeQuad(Quad& quad, sf::IntRect const& rect);

    private:
        std::vector<Particle> mParticles;
        std::vector<Affector> mAffectors;

        sf::Texture* mTexture;
        std::vector<sf::IntRect> mTextureRects;

        sf::VertexArray mVertices;
        bool mNeedsVertexUpdate;
        std::vector<Quad> mQuads;
        bool mNeedsQuadUpdate;
};

#endif // NPARTICLESYSTEM_HPP
