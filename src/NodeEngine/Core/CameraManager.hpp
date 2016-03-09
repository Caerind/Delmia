#ifndef NCAMERAMANAGER_HPP
#define NCAMERAMANAGER_HPP

#include "CameraComponent.hpp"
#include "../Utils/Array.hpp"

class NCameraManager
{
    public:
        NCameraManager();
        ~NCameraManager();

        void addCamera(NCameraComponent* camera);
        void removeCamera(NCameraComponent* camera);

        sf::View& getView();

        sf::View getActiveView() const;

    protected:
        NArray<NCameraComponent*> mCameras;
        sf::View mView;
        int mActive;
};

#endif // NCAMERAMANAGER_HPP
