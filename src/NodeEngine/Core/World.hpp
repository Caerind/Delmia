#ifndef NWORLD_HPP
#define NWORLD_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "CameraManager.hpp"
#include "Tickable.hpp"
#include "Actor.hpp"
#include "Action.hpp"
#include "SceneComponent.hpp"

#include "../Utils/Array.hpp"
#include "../Utils/Map.hpp"
#include "../Utils/Pugixml.hpp"
#include "../Utils/Timer.hpp"

#include "../Application/Application.hpp"

class NWorld
{
    public:
        static NWorld& instance();

        // Add an event to the list
        static void addEvent(sf::Event const& event);

        // Test if an event exist
        //static bool testEvent(sf::Event const& event);
        static bool testAction(NAction const& action);

        // Tick the world
        static void tick(sf::Time dt);

        // Render the world
        static void render(sf::RenderTarget& target);

        // Update the world : use temp array and then clear them
        static void update();

        template<typename T>
        static bool registerActor(std::string const& type);

        template <typename T>
        static std::shared_ptr<T> createActor();

        // Add an actor
        static void addActor(NActor::Ptr actor);

        // Get an actor
        static NActor::Ptr getActor(std::size_t index);
        static NActor::Ptr getActor(std::string const& id);

        // Remove an actor
        static void removeActor(std::string const& id);

        static bool load(std::string const& filename);
        static bool save(std::string const& filename);

        // CameraManager
        static NCameraManager& getCameraManager();

        static std::size_t getActorCount();
        static std::size_t getRenderableCount();
        static std::size_t getTickableCount();

        static NVector getPointerPositionScreen(int touchIndex = 0);
        static NVector getPointerPositionView(int touchIndex = 0);

        static ah::ResourceManager& getResources();
        static ah::Window& getWindow();

        static std::string setTimer(sf::Time duration, NTimer::Callback function = [](){});
        static sf::Time getTimerRemaining(std::string const& handle);
        static sf::Time getTimerDuration(std::string const& handle);
        static void resetTimer(std::string const& handle, sf::Time newDuration);
        static void stopTimer(std::string const& handle);

        /* These following arent static cause they shouldn't be use directly */
        void addRenderable(NSceneComponent* renderable);
        void removeRenderable(NSceneComponent* renderable);
        void addTickable(NTickable* tickable);
        void removeTickable(NTickable* tickable);

    private:
        NWorld();
        ~NWorld();

        static NWorld* mInstance;

    private:
        NArray<sf::Event> mEvents;

        NArray<NActor::Ptr> mActors;
        NArray<std::string> mActorsDeletions;

        NArray<NSceneComponent*> mRenderables;
        NArray<NSceneComponent*> mRenderablesAdditions;
        NArray<NSceneComponent*> mRenderablesDeletions;

        NArray<NTickable*> mTickables;
        NArray<NTickable*> mTickablesAdditions;
        NArray<NTickable*> mTickablesDeletions;

        NMap<std::string,NTimer> mTimers;

        NCameraManager mCameraManager;

        NMap<std::string,std::function<NActor::Ptr()>> mActorFactory;
};

template <typename T>
std::shared_ptr<T> NWorld::createActor()
{
    std::shared_ptr<T> actor = std::make_shared<T>();
    instance().mActors.add(actor);
    return actor;
}

template<typename T>
bool NWorld::registerActor(std::string const& type)
{
	instance().mActorFactory[type] = [] ()
	{
		return NActor::Ptr(new T());
	};
	return true;
}

#endif // NWORLD_HPP
