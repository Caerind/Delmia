#include "GraphicManager.hpp"

NGraphicManager::NGraphicManager() : mEffect(nullptr)
{
    mRenderOrder = [](NSceneComponent* a, NSceneComponent* b) -> bool
    {
        if (a != nullptr && b != nullptr)
        {
            if (a->getFinalZ() < b->getFinalZ())
            {
                return true;
            }
            else if (a->getFinalZ() > b->getFinalZ())
            {
                return false;
            }
            else if (a->getFinalPosition().y < b->getFinalPosition().y)
            {
                return true;
            }
            else if (a->getFinalPosition().y > b->getFinalPosition().y)
            {
                return false;
            }
            else
            {
                return (a->getFinalPosition().x < b->getFinalPosition().x);
            }
        }
        return true;
    };

    mNeedUpdateOrder = true;
}

void NGraphicManager::updateRenderSize(sf::Vector2u const& size)
{
    mSceneTexture.clear();
    mSceneTexture.create(size.x,size.y);
    mSceneTexture.clear();
}

void NGraphicManager::render(sf::RenderTarget& target)
{
    if (mNeedUpdateOrder)
    {
        std::sort(mRenderables.begin(),mRenderables.end(),mRenderOrder);
        mNeedUpdateOrder = false;
    }

    if (NEffect::isSupported() && mEffect != nullptr)
    {
        mSceneTexture.clear();
        renderScene(mSceneTexture);
        mSceneTexture.display();
        mEffect->apply(mSceneTexture,target);
    }
    else
    {
        renderScene(target);
    }
}

void NGraphicManager::clear()
{
    mRenderables.clear();
    mParticleSystems.clear();
    mCameraManager.clear();
    mSceneTexture.clear();
    if (mEffect != nullptr)
    {
        delete mEffect;
        mEffect = nullptr;
    }
    mNeedUpdateOrder = false;
}

NCameraManager& NGraphicManager::getCameraManager()
{
    return mCameraManager;
}

sf::View& NGraphicManager::getActiveView()
{
    return mCameraManager.getActiveView();
}

sf::View& NGraphicManager::getView()
{
    return mCameraManager.getView();
}

std::size_t NGraphicManager::getRenderableCount() const
{
    return mRenderables.size();
}

void NGraphicManager::addRenderable(NSceneComponent* renderable)
{
    mRenderables.push_back(renderable);
    needUpdateOrder();
}

void NGraphicManager::removeRenderable(NSceneComponent* renderable)
{
    remove(mRenderables,renderable);
    needUpdateOrder();
}

NParticleSystem::Ptr NGraphicManager::getParticleSystem(std::string const& systemId)
{
    if (!contains(mParticleSystems,systemId))
    {
        mParticleSystems[systemId] = std::make_shared<NParticleSystem>();
    }
    return mParticleSystems.at(systemId);
}

void NGraphicManager::removeParticleSystem(std::string const& systemId)
{
    remove(mParticleSystems,systemId);
}

std::size_t NGraphicManager::getParticleSystemCount() const
{
    return mParticleSystems.size();
}

void NGraphicManager::needUpdateOrder()
{
    mNeedUpdateOrder = true;
}

void NGraphicManager::renderScene(sf::RenderTarget& target)
{
    sf::View old = target.getView();
    target.setView(getActiveView());
    for (auto itr = mRenderables.begin(); itr != mRenderables.end(); itr++)
    {
        (*itr)->render(target);
    }
    target.setView(old);
}
