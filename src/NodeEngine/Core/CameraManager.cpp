#include "CameraManager.hpp"
#include "World.hpp"

NCameraManager::NCameraManager()
{
    mActive = -1;
    mView = NWorld::getWindow().getView();
}

NCameraManager::~NCameraManager()
{
}

void NCameraManager::addCamera(NCameraComponent* camera)
{
    mCameras.add(camera);
    if (mActive == -1 && camera != nullptr)
    {
        mActive = (int)mCameras.size() - 1;
    }
}

void NCameraManager::removeCamera(NCameraComponent* camera)
{
    mCameras.remove(camera);
}

sf::View& NCameraManager::getView()
{
    return mView;
}

sf::View& NCameraManager::getActiveView()
{
    if (mActive >= 0 && mActive < (int)mCameras.size())
    {
        if (mCameras[mActive] != nullptr)
        {
            return mCameras[mActive]->getView();
        }
    }
    return mView;
}

void NCameraManager::clear()
{
    mCameras.clear();
    mActive = -1;
    mView = NWorld::getWindow().getView();
}
