#ifndef NWORLD_HPP
#define NWORLD_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>

#include "Tickable.hpp"
#include "Actor.hpp"
#include "GraphicManager.hpp"

#include "../Utils/Array.hpp"
#include "../Utils/Map.hpp"
#include "../Utils/Pugixml.hpp"
#include "../Utils/Timer.hpp"
#include "../Utils/Log.hpp"

#include "../Application/Application.hpp"

class NWorld
{
    public:
        static NWorld& instance();

        // Add an event to the list
        static void addEvent(sf::Event const& event);

        // Tick the world
        static void tick(sf::Time dt);

        // Render the world
        static void render();

        // Remove All
        static void clear();

        #ifdef N_DESKTOP_PLATFORM
        template <typename T>
        static bool registerActor();
        #endif // N_DESKTOP_PLATFORM

        template<typename T>
        static bool registerActor(std::string const& type);

        template <typename T, typename ... Args>
        static std::shared_ptr<T> createActor(Args&& ... args);

        // Get an actor
        static NActor::Ptr getActor(std::size_t index);
        static NActor::Ptr getActor(std::string const& id);

        // Remove an actor
        static void removeActor(std::size_t index);
        static void removeActor(std::string const& id);

        static bool load(std::string const& filename);
        static bool save(std::string const& filename);

        // CameraManager
        static NCameraManager& getCameraManager();
        static sf::View& getActiveView();

        static std::size_t getActorCount();
        static std::size_t getRenderableCount();
        static std::size_t getTickableCount();

        static NVector getPointerPositionScreen(int touchIndex = 0);
        static NVector getPointerPositionView(int touchIndex = 0);

        static ah::ResourceManager& getResources();
        static ah::Window& getWindow();
        static ah::ValueContainer& getValues();

        static std::string setTimer(sf::Time duration, NTimer::Callback function = [](){});
        static std::string startTimer();
        static sf::Time getTimerElapsed(std::string const& handle);
        static sf::Time getTimerRemaining(std::string const& handle);
        static sf::Time getTimerDuration(std::string const& handle);
        static void repeatTimer(std::string const& handle, bool repeat);
        static void playTimer(std::string const& handle);
        static void pauseTimer(std::string const& handle);
        static void resetTimer(std::string const& handle, sf::Time newDuration);
        static void stopTimer(std::string const& handle);

        /* These following arent static cause they shouldn't be use directly */
        void addRenderable(NSceneComponent* renderable);
        void removeRenderable(NSceneComponent* renderable);
        void addTickable(NTickable* tickable);
        void removeTickable(NTickable* tickable);

        static NParticleSystem::Ptr getParticleSystem(std::string const& systemId);
        static void removeParticleSystem(std::string const& systemId);
        static std::size_t getParticleSystemCount();

        template <typename T>
        static void setEffect();

        template <typename T>
        static T* getEffect();

        static void needUpdateOrder();

    private:
        NWorld();
        ~NWorld();

        static NWorld* mInstance;

    private:
        NGraphicManager mGraphics;

        NArray<sf::Event> mEvents;

        NArray<NActor::Ptr> mActors;
        NArray<std::string> mActorsDeletions;

        NArray<NTickable*> mTickables;

        NMap<std::string,NTimer> mTimers;
        int mTimerHandleCounter;

        NMap<std::string,std::function<NActor::Ptr()>> mActorFactory;
};

template <typename T, typename ... Args>
std::shared_ptr<T> NWorld::createActor(Args&& ... args)
{
    std::shared_ptr<T> actor = std::make_shared<T>(std::forward<Args>(args)...);
    instance().mActors.add(actor);
    return actor;
}

#ifdef N_DESKTOP_PLATFORM
template <typename T>
bool NWorld::registerActor()
{
    return registerActor<T>(NString::type<T>());
}
#endif // N_DESKTOP_PLATFORM

template<typename T>
bool NWorld::registerActor(std::string const& type)
{
	instance().mActorFactory[type] = [] ()
	{
		return NActor::Ptr(new T());
	};
	return true;
}

template <typename T>
void NWorld::setEffect()
{
    instance().mGraphics.setEffect<T>();
}

template <typename T>
T* NWorld::getEffect()
{
    return instance().mGraphics.getEffect<T>();
}

#endif // NWORLD_HPP
