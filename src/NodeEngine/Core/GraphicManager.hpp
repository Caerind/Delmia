#ifndef NGRAPHICMANAGER_HPP
#define NGRAPHICMANAGER_HPP

#include <functional>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "../Utils/Array.hpp"
#include "../Utils/Map.hpp"

#include "CameraManager.hpp"
#include "Effect.hpp"
#include "ParticleSystem.hpp"

class NGraphicManager
{
    public:
        NGraphicManager();

        void updateRenderSize(sf::Vector2u const& size);

        void render(sf::RenderTarget& target);

        void clear();

        NCameraManager& getCameraManager();
        sf::View& getActiveView();
        sf::View& getView();

        std::size_t getRenderableCount() const;

        void addRenderable(NSceneComponent* renderable);
        void removeRenderable(NSceneComponent* renderable);

        NParticleSystem::Ptr getParticleSystem(std::string const& systemId);
        void removeParticleSystem(std::string const& systemId);
        std::size_t getParticleSystemCount() const;

        template <typename T>
        void setEffect();

        template <typename T>
        T* getEffect();

        void needUpdateOrder();

    private:
        void renderScene(sf::RenderTarget& target);

    private:
        NArray<NSceneComponent*> mRenderables;

        NMap<std::string,NParticleSystem::Ptr> mParticleSystems;

        NCameraManager mCameraManager;

        sf::RenderTexture mSceneTexture;
        NEffect* mEffect;

        std::function<bool(NSceneComponent*,NSceneComponent*)> mRenderOrder;
        bool mNeedUpdateOrder;
};

template <typename T>
void NGraphicManager::setEffect()
{
    mEffect = new T();
}

template <typename T>
T* NGraphicManager::getEffect()
{
    return static_cast<T*>(mEffect);
}

#endif // NGRAPHICMANAGER_HPP
