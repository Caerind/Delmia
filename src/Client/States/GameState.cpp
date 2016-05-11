#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mCameraSpeed = 1000.f;

    mPlacing = false;
    mPlacingType = 0;
    mPlacement = nullptr;

    mSelecting = false;
    mSelectionZone.setFillColor(sf::Color::Transparent);
    mSelectionZone.setOutlineColor(sf::Color::Black);
    mSelectionZone.setOutlineThickness(2.f);

    mWorld.createResource<Quarry>(sf::Vector2i(10,10));
    mWorld.createResource<Forest>(sf::Vector2i(14,14));
    mWorld.createResource<GoldMine>(sf::Vector2i(6,6));
}

bool GameState::handleEvent(sf::Event const& event)
{
    mWorld.handleEvent(event);

    handleZoom(event);
    handlePlacement(event);
    handleSelection(event);
    handleUnit(event);

    return true;
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    handleViewMovement(dt);

    return true;
}

void GameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
    sf::View v = target.getView();
    target.setView(NWorld::getActiveView());

    mWorld.render(target);

    if (mSelecting)
    {
        target.draw(mSelectionZone);
    }

    target.setView(v);
}

void GameState::handleZoom(sf::Event const& event)
{
    // TODO : Limit size of the view
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        if (event.mouseWheelScroll.delta < 1)
        {
            NWorld::getCameraManager().getView().zoom(1.2f);
        }
        else
        {
            NWorld::getCameraManager().getView().zoom(0.8f);
        }
    }
}

void GameState::handlePlacement(sf::Event const& event)
{
    // Actual Mouse Coords
    sf::Vector2i c = mWorld.getMouseCoords();

    // Middle Click Pressed
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle && !mPlacing)
    {
        mPlacing = true;
        switch (mPlacingType)
        {
            case 0: mPlacement = mWorld.createActor<Hall>(nullptr,c); break;
            case 1: mPlacement = mWorld.createActor<Market>(nullptr,c); break;
            case 2: mPlacement = mWorld.createActor<Barrack>(nullptr,c); break;
            default: mPlacement = nullptr; break;
        }
        if (mPlacement != nullptr)
        {
            mPlacement->setPositionZ(100000.f);
            mPlacement->generate(c);
            bool collide;
            switch (mPlacingType)
            {
                case 0: collide = !mWorld.buildingPlacing<Hall>(c); break;
                case 1: collide = !mWorld.buildingPlacing<Market>(c); break;
                case 2: collide = !mWorld.buildingPlacing<Barrack>(c); break;
                default: collide = false; break;
            }
            mPlacement->setColor((collide) ? sf::Color::Red : sf::Color::Green);
            mPlacement->setBuilt(true);
        }
    }

    // Middle Click Released
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle && mPlacing)
    {
        mPlacing = false;
        if (mPlacement != nullptr)
        {
            mWorld.removeActor(mPlacement->getId());
            mPlacement = nullptr;
            switch (mPlacingType)
            {
                case 0: mWorld.createBuilding<Hall>(mWorld.getLocalPlayerId(),c); break;
                case 1: mWorld.createBuilding<Market>(mWorld.getLocalPlayerId(),c); break;
                case 2: mWorld.createBuilding<Barrack>(mWorld.getLocalPlayerId(),c); break;
                default: break;
            }
        }
    }

    // Mouse Moved
    if (event.type == sf::Event::MouseMoved && mPlacing && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    {
        if (mPlacement != nullptr)
        {
            if (mPlacement->getCoords() != c)
            {
                mPlacement->generate(c);
                bool collide;
                switch (mPlacingType)
                {
                    case 0: collide = !mWorld.buildingPlacing<Hall>(c); break;
                    case 1: collide = !mWorld.buildingPlacing<Market>(c); break;
                    case 2: collide = !mWorld.buildingPlacing<Barrack>(c); break;
                    default: collide = false; break;
                }
                mPlacement->setColor((collide) ? sf::Color::Red : sf::Color::Green);
            }
        }
    }

    // Press B
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
    {
        mPlacingType = (mPlacingType + 1) % 3;
    }
}

void GameState::handleSelection(sf::Event const& event)
{
    sf::Vector2f p = NWorld::getPointerPositionView();
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !mSelecting)
    {
        mSelecting = true;
        mSelectionZone.setPosition(p);
        mSelected.clear();
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mSelecting)
    {
        mSelected = mWorld.selectUnits(mSelectionZone.getGlobalBounds());
        if (mSelected.size() == 0)
        {
            Building::Ptr building = mWorld.getBuilding(NIsometric::worldToCoords(p));
            if (building != nullptr)
            {
                // TODO : Open Building Window
            }
        }
        mSelecting = false;
        mSelectionZone.setSize({0.f, 0.f});
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mSelecting && event.type == sf::Event::MouseMoved)
    {
        mSelectionZone.setSize(p - mSelectionZone.getPosition());
    }
}

void GameState::handleUnit(sf::Event const& event)
{
    sf::Vector2f p = NWorld::getPointerPositionView();

    // TEMPORARY Add an unit
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)
    {
        mWorld.createUnit<Unit>(mWorld.getLocalPlayerId(),p);
    }


    if (mSelected.size() > 0 && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
    {
        sf::Vector2f pos = NWorld::getPointerPositionView();
        sf::Vector2i coords = NIsometric::worldToCoords(pos);
        Building::Ptr building = mWorld.getBuilding(coords);
        Resource::Ptr resource = mWorld.getResource(coords);
        Unit::Ptr unit = mWorld.getUnit(pos);
        bool handled = false;
        if (building != nullptr)
        {
            if (building->getOwnerId() == mWorld.getLocalPlayerId())
            {
                if (building->isBuilt())
                {
                    for (std::size_t i = 0; i < mSelected.size(); i++)
                    {
                        // Give resources
                        //mSelected[i]->giveResource(building.get());
                    }
                }
                else
                {
                    for (std::size_t i = 0; i < mSelected.size(); i++)
                    {
                        if (mSelected[i]->getType() == Units::Citizen)
                        {
                            // Build it
                            //mSelected[i]->build(building.get());
                        }
                    }
                }
            }
            else
            {
                for (std::size_t i = 0; i < mSelected.size(); i++)
                {
                    if (mSelected[i]->getType() == Units::Soldier)
                    {
                        // Attack and rob
                    }
                }
            }
            handled = true;
        }
        else if (resource != nullptr)
        {
            for (std::size_t i = 0; i < mSelected.size(); i++)
            {
                if (mSelected[i]->getType() == Units::Citizen)
                {
                    // Gather resources
                    mSelected[i]->positionOrder(resource->getEntryPoint());
                    //mSelected[i]->gatherResource(resource.get());
                }
            }
            handled = true;
        }
        else if (unit != nullptr)
        {
            if (unit->getOwnerId() == mWorld.getLocalPlayerId())
            {
                for (std::size_t i = 0; i < mSelected.size(); i++)
                {
                    if (mSelected[i]->getType() == Units::Soldier)
                    {
                        // Attack and rob
                    }
                }
                handled = true;
            }
        }

        if (!handled)
        {
            // Go
            for (std::size_t i = 0; i < mSelected.size(); i++)
            {
                if (!mWorld.collide(NIsometric::worldToCoords(pos)))
                {
                    if (i > 0)
                    {
                        bool ok = false;
                        do
                        {
                            pos = NWorld::getPointerPositionView() + sf::Vector2f(NMath::randomDev(0.f,40.f),NMath::randomDev(0.f,40.f));
                            if (!mWorld.collide(NIsometric::worldToCoords(pos)))
                            {
                                ok = true;
                            }
                        } while (!ok);
                    }
                    mSelected[i]->positionOrder(pos);
                }
            }
        }
    }
}

void GameState::handleViewMovement(sf::Time dt)
{
    sf::Vector2f mvt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        mvt.y--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        mvt.x--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mvt.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mvt.x++;
    }
    NWorld::getCameraManager().getView().move(dt.asSeconds() * mCameraSpeed * mvt);
}

float GameState::getCameraSpeed()
{
    return mCameraSpeed * getCameraZoom();
}

float GameState::getCameraZoom()
{
    return 1.f;
}

void GameState::onDeactivate()
{
    mWorld.clear();
}
