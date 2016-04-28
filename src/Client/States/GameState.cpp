#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mPlacing = false;
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
    // Coordonnées
    sf::Vector2i c = mWorld.getMouseCoords();

    // Left Click
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        mPlacing = true;
        switch (mPlacingType)
        {
            case 0: mPlacement = mWorld.createActor<Forest>(c.x,c.y); break;
            case 1: mPlacement = mWorld.createActor<GoldMine>(c.x,c.y); break;
            case 2: mPlacement = mWorld.createActor<Quarry>(c.x,c.y); break;
            case 3: mPlacement = mWorld.createActor<Hall>(c.x,c.y); break;
            case 4: mPlacement = mWorld.createActor<Market>(c.x,c.y); break;
            case 5: mPlacement = mWorld.createActor<Barrack>(c.x,c.y); break;
            default: break;
        }
        mPlacement->setPositionZ(100.f);
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        mPlacing = false;
        NWorld::removeActor(mPlacement->getId());
        mPlacement = nullptr;

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

    // Mouse Moved
    if (event.type == sf::Event::MouseMoved && mPlacing)
    {
        if (mPlacement->getCoords() != c)
        {
            mPlacement->clearTiles();
            mPlacement->generate(c.x,c.y);
            if (0 <= mPlacingType && mPlacingType < 3)
            {
                if (mWorld.collide(c.x,c.y))
                {
                    mPlacement->setColor(sf::Color::Red);
                }
                else
                {
                    mPlacement->setColor(sf::Color::Green);
                }
            }
            else
            {
                bool collide = false;
                std::vector<sf::Vector2i> tiles;
                switch (mPlacingType)
                {
                    case 3: tiles = Hall::getTilesBlueprint(c.x,c.y); break;
                    case 4: tiles = Market::getTilesBlueprint(c.x,c.y); break;
                    case 5: tiles = Barrack::getTilesBlueprint(c.x,c.y); break;
                    default: break;
                }
                for (auto v : tiles)
                {
                    if (mWorld.collide(v.x,v.y))
                    {
                        collide = true;
                        mPlacement->setColor(sf::Color::Red);
                    }
                    std::vector<sf::Vector2i> ns = NMapUtility::Isometric::getNeighboors(v,true);
                    for (auto n : ns)
                    {
                        if (mWorld.collide(n.x,n.y,false))
                        {
                            collide = true;
                            mPlacement->setColor(sf::Color::Red);
                        }
                    }
                }
                if (!collide)
                {
                    mPlacement->setColor(sf::Color::Green);
                }
            }
        }
    }

    // Right click
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && !mPlacing)
    {
        mPlacingType = (mPlacingType >= 6)? 0 : mPlacingType+1;
    }
}
