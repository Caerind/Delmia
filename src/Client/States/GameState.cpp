#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mPlacing = false;
    mPlacingCollide = false;
    mPlacingType = 0;
    mPlacement = nullptr;
}

bool GameState::handleEvent(sf::Event const& event)
{
    mWorld.handleEvent(event);

    handlePlacement(event);

    return true;
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    return true;
}

void GameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
    mWorld.render(target);
}

void GameState::handlePlacement(sf::Event const& event)
{
    // Actual Mouse Coords
    sf::Vector2i c = mWorld.getMouseCoords();

    // Left Click Pressed
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !mPlacing)
    {
        mPlacing = true;
        switch (mPlacingType)
        {
            case 0: mPlacement = mWorld.createActor<Forest>(); break;
            case 1: mPlacement = mWorld.createActor<GoldMine>(); break;
            case 2: mPlacement = mWorld.createActor<Quarry>(); break;
            case 3: mPlacement = mWorld.createActor<Hall>(); break;
            case 4: mPlacement = mWorld.createActor<Market>(); break;
            case 5: mPlacement = mWorld.createActor<Barrack>(); break;
            default: mPlacement = nullptr; break;
        }
        if (mPlacement != nullptr)
        {
            mPlacement->setPositionZ(100000.f);
            movePlacement(c);
            mPlacement->setBuilt(true);
        }
        else
        {
            mPlacing = false;
            mPlacingCollide = false;
        }
    }

    // Left Click Released
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mPlacing)
    {
        mPlacing = false;
        NWorld::removeActor(mPlacement->getId());
        mPlacement = nullptr;

        if (!mPlacingCollide)
        {
            switch (mPlacingType)
            {
                case 0: mWorld.createResource<Forest>(c.x,c.y); break;
                case 1: mWorld.createResource<GoldMine>(c.x,c.y); break;
                case 2: mWorld.createResource<Quarry>(c.x,c.y); break;
                case 3: mWorld.createBuilding<Hall>(c.x,c.y); break;
                case 4: mWorld.createBuilding<Market>(c.x,c.y); break;
                case 5: mWorld.createBuilding<Barrack>(c.x,c.y); break;
                default: break;
            }
        }
        mPlacingCollide = false;
    }

    // Mouse Moved
    if (event.type == sf::Event::MouseMoved && mPlacing)
    {
        if (mPlacement->getCoords() != c)
        {
            movePlacement(c);
        }
    }

    // Right click
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && !mPlacing)
    {
        mPlacingType = (mPlacingType >= 6) ? 0 : mPlacingType + 1;
    }
}

void GameState::movePlacement(sf::Vector2i const& coords)
{
    mPlacement->clearTiles();
    mPlacement->generate(coords.x,coords.y);
    switch (mPlacingType)
    {
        case 0: mPlacingCollide = mWorld.collide(coords.x,coords.y); break;
        case 1: mPlacingCollide = mWorld.collide(coords.x,coords.y); break;
        case 2: mPlacingCollide = mWorld.collide(coords.x,coords.y); break;
        case 3: mPlacingCollide = !mWorld.buildingPlacing<Hall>(coords.x,coords.y); break;
        case 4: mPlacingCollide = !mWorld.buildingPlacing<Market>(coords.x,coords.y); break;
        case 5: mPlacingCollide = !mWorld.buildingPlacing<Barrack>(coords.x,coords.y); break;
        default: mPlacingCollide = false; break;
    }
    mPlacement->setColor((mPlacingCollide) ? sf::Color::Red : sf::Color::Green);
}

void GameState::onDeactivate()
{
    mWorld.clear();
}
