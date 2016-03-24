#include "World.hpp"

NWorld* NWorld::mInstance = nullptr;

NWorld& NWorld::instance()
{
	if (mInstance == nullptr)
	{
		mInstance = new NWorld();
	}
    return *mInstance;
}

void NWorld::addEvent(sf::Event const& event)
{
    instance().mEvents.add(event);
}

/*
bool NWorld::testEvent(sf::Event const& event)
{
    for (std::size_t i = 0; i < instance().mEvents.size(); i++)
    {
        if (event == instance().mEvents[i])
        {
            return true;
        }
    }
    return false;
}*/

bool NWorld::testAction(NAction const& action)
{
    for (std::size_t i = 0; i < instance().mEvents.size(); i++)
    {
        if (action == instance().mEvents[i])
        {
            return true;
        }
    }
    return false;
}

void NWorld::tick(sf::Time dt)
{
    for (auto itr = instance().mTickables.begin(); itr != instance().mTickables.end(); itr++)
    {
        (*itr)->tick(dt);
    }

    for (auto itr = instance().mTimers.begin(); itr != instance().mTimers.end(); itr++)
    {
        itr->second.update(dt);
    }
}

void NWorld::render(sf::RenderTarget& target)
{
    instance().mRenderables.sort([](NSceneComponent* a, NSceneComponent* b) -> bool
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
           else
           {
               return (a->getFinalPosition().y < b->getFinalPosition().y);
           }
       }
       return true;
    });

    sf::View old = target.getView();
    target.setView(instance().mCameraManager.getActiveView());
    for (auto itr = instance().mRenderables.begin(); itr != instance().mRenderables.end(); itr++)
    {
        (*itr)->render(target);
    }
    target.setView(old);
}

void NWorld::update()
{
    // Events
    instance().mEvents.clear();

    // Renderables
    for (std::size_t i = 0; i < instance().mRenderablesDeletions.size(); i++)
    {
        instance().mRenderables.remove(instance().mRenderablesDeletions[i]);
    }
    instance().mRenderablesDeletions.clear();

    // Tickables
    for (std::size_t i = 0; i < instance().mTickablesAdditions.size(); i++)
    {
        instance().mTickables.add(instance().mTickablesAdditions[i]);
    }
    instance().mTickablesAdditions.clear();
    for (std::size_t i = 0; i < instance().mTickablesDeletions.size(); i++)
    {
        instance().mTickables.remove(instance().mTickablesDeletions[i]);
    }
    instance().mTickablesDeletions.clear();
}

NActor::Ptr NWorld::getActor(std::size_t index)
{
    if (index >= 0 && index < instance().mActors.size())
    {
        return instance().mActors[index];
    }
    return nullptr;
}

NActor::Ptr NWorld::getActor(std::string const& id)
{
    std::size_t size = instance().mActors.size();
    for (std::size_t i = 0; i < size; i++)
    {
        if (instance().mActors[i]->getId() == id)
        {
            return instance().mActors[i];
        }
    }
    return nullptr;
}

void NWorld::removeActor(std::string const& id)
{
    for (std::size_t i = 0; i < instance().mActors.size(); i++)
    {
        if (instance().mActors[i]->getId() == id)
        {
            instance().mActors.erase(i);
        }
    }
}

bool NWorld::load(std::string const& filename)
{
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        return false;
    }
    pugi::xml_node actors = doc.child("Actors");
    if (!actors)
    {
        return false;
    }
    for (pugi::xml_node actor = actors.first_child(); actor; actor = actor.next_sibling())
    {
        std::string type = actor.attribute("type").value();
        if (instance().mActorFactory.contains(type))
        {
            auto a = instance().mActorFactory[type]();
            a->load(actor);
            a->NActor::load(actor);
            instance().mActors.add(a);
        }
    }

    return true;
}

void NWorld::addActor(NActor::Ptr actor)
{
    instance().mActors.add(actor);
}

bool NWorld::save(std::string const& filename)
{
    pugi::xml_document doc;
    pugi::xml_node actors = doc.append_child("Actors");
    std::size_t size = getActorCount();
    for (std::size_t i = 0; i < size; i++)
    {
        pugi::xml_node actor = actors.append_child("Actor");
        if (actor)
        {
            auto a = getActor(i);
            if (a != nullptr)
            {
                a->save(actor);
                a->NActor::save(actor);
            }
        }
    }
    doc.save_file(filename.c_str());
    return true;
}

NCameraManager& NWorld::getCameraManager()
{
    return instance().mCameraManager;
}

std::size_t NWorld::getActorCount()
{
    return instance().mActors.size();
}

std::size_t NWorld::getRenderableCount()
{
    return instance().mRenderables.size();
}

std::size_t NWorld::getTickableCount()
{
    return instance().mTickables.size();
}

NVector NWorld::getPointerPositionScreen(int touchIndex)
{
    return NVector::SFML2FToN(getWindow().getPointerPosition(touchIndex));
}

NVector NWorld::getPointerPositionView(int touchIndex)
{
    return NVector::SFML2FToN(getWindow().getPointerPositionView(instance().mCameraManager.getActiveView(),touchIndex));
}

ah::ResourceManager& NWorld::getResources()
{
    return ah::Application::getResources();
}

ah::Window& NWorld::getWindow()
{
    return ah::Application::getWindow();
}

std::string NWorld::setTimer(sf::Time duration, NTimer::Callback callback)
{
    static int x = 0;
    x++;
    std::string handle = std::to_string(x);
    instance().mTimers[handle].setCallback(callback);
    instance().mTimers[handle].reset(duration);
    return handle;
}

sf::Time NWorld::getTimerRemaining(std::string const& handle)
{
    if (instance().mTimers.contains(handle))
    {
        return instance().mTimers[handle].getRemaining();
    }
    return sf::Time::Zero;
}

sf::Time NWorld::getTimerDuration(std::string const& handle)
{
    if (instance().mTimers.contains(handle))
    {
        return instance().mTimers[handle].getDuration();
    }
    return sf::Time::Zero;
}

void NWorld::resetTimer(std::string const& handle, sf::Time newDuration)
{
    if (instance().mTimers.contains(handle))
    {
        instance().mTimers[handle].reset(newDuration);
    }
}

void NWorld::stopTimer(std::string const& handle)
{
    instance().mTimers.remove(handle);
}

void NWorld::addRenderable(NSceneComponent* renderable)
{
    instance().mRenderables.add(renderable);
}

void NWorld::removeRenderable(NSceneComponent* renderable)
{
    mRenderablesDeletions.add(renderable);
}

void NWorld::addTickable(NTickable* tickable)
{
    mTickablesAdditions.add(tickable);
}

void NWorld::removeTickable(NTickable* tickable)
{
    mTickablesDeletions.add(tickable);
}

NWorld::NWorld()
{
}

NWorld::~NWorld()
{
}
