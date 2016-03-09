#include "Application.hpp"

namespace ah
{

Application* Application::mInstance = nullptr;

Application& Application::instance()
{
    if (mInstance == nullptr)
    {
        mInstance = new Application();
    }
    return *mInstance;
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate;
    sf::Time timePerFrame = sf::seconds(1/60.f);
    sf::Clock fpsClock;
    std::size_t fps = 0;
    instance().mWindow.setDebugInfo("FPS","0");
    instance().mRunning = true;
    while (instance().mRunning)
    {
        timeSinceLastUpdate += clock.restart();

        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            sf::Event event;
            while (instance().mWindow.pollEvent(event))
            {
                instance().mStates.handleEvent(event);

                if (event.type == sf::Event::Closed)
                {
                    close();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3)
                {
                    instance().mWindow.showDebugInfo(!instance().mWindow.isDebugInfoVisible());
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2)
                {
                    instance().mWindow.screenshot();
                }
            }

            instance().mStates.update(timePerFrame);
            if (instance().mStates.empty())
            {
                close();
            }
            instance().mAudio.update();
        }

        instance().mWindow.clear();
        instance().mStates.render(instance().mWindow,sf::RenderStates());
        instance().mWindow.display();

        fps++;
        if (fpsClock.getElapsedTime() >= sf::seconds(1.f))
        {
            instance().mWindow.setDebugInfo("FPS",std::to_string(fps));
            fps = 0;
            fpsClock.restart();
        }
    }
    instance().mWindow.close();
}

void Application::close()
{
    instance().mRunning = false;
}

StateManager& Application::getStates()
{
    return instance().mStates;
}

Window& Application::getWindow()
{
    return instance().mWindow;
}

AudioManager& Application::getAudio()
{
    return instance().mAudio;
}

ResourceManager& Application::getResources()
{
    return instance().mResources;
}

LangManager& Application::getLang()
{
    return instance().mLang;
}

Application::Application()
{
    mRunning = false;
}

Application::~Application()
{
}

} // namespace ah
