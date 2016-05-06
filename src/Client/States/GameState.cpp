#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mPlacing = false;
    mPlacingCollide = false;
    mPlacingType = 0;
    mPlacement = nullptr;

    mSelecting = false;
    mSelectionZone.setFillColor(sf::Color::Transparent);
    mSelectionZone.setOutlineColor(sf::Color::Black);
    mSelectionZone.setOutlineThickness(2.f);
}

bool GameState::handleEvent(sf::Event const& event)
{
    mWorld.handleEvent(event);

    handleZoom(event);
    handlePlacement(event);
    handleUnit(event);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        if (oldC != sf::Vector2i())
        {
            std::vector<sf::Vector2i> p = NMapUtility::pathfinding(NMapUtility::Type::Isometric,oldC,mWorld.getMouseCoords());
            for (std::size_t i = 0; i < p.size(); i++)
            {
                std::cout << p[i].x << " " << p[i].y << std::endl;
            }
        }

        oldC = mWorld.getMouseCoords();
    }

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

    // Left Click Pressed
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !mPlacing)
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
    }

    // Left Click Released
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && mPlacing)
    {
        mPlacing = false;
        if (mPlacement != nullptr)
        {
            mWorld.removeActor(mPlacement->getId());
            mPlacement = nullptr;
        }

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
    if (event.type == sf::Event::MouseMoved && mPlacing && sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (mPlacement != nullptr)
        {
            if (mPlacement->getCoords() != c)
            {
                movePlacement(c);
            }
        }
    }

    // Press A
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
    {
        mPlacingType = (mPlacingType >= 6) ? 0 : mPlacingType + 1;
        if (mPlacing)
        {
            if (mPlacement != nullptr)
            {
                mWorld.removeActor(mPlacement->getId());
            }
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
        }
    }
}

void GameState::movePlacement(sf::Vector2i const& coords)
{
    if (mPlacement != nullptr)
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
}

void GameState::handleUnit(sf::Event const& event)
{
    sf::Vector2f p = NWorld::getPointerPositionView();

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)
    {
        mWorld.createUnit<Unit>(p.x,p.y);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !mSelecting)
    {
        mSelecting = true;
        mSelectionZone.setPosition(p);
        mSelectedUnits.clear();
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mSelecting)
    {
        mSelectedUnits = mWorld.selectUnits(mSelectionZone.getGlobalBounds());
        mSelecting = false;
        mSelectionZone.setSize({0.f, 0.f});
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mSelecting && event.type == sf::Event::MouseMoved)
    {
        mSelectionZone.setSize(p - mSelectionZone.getPosition());
    }

    if (mSelectedUnits.size() > 0 && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle)
    {
        for (std::size_t i = 0; i < mSelectedUnits.size(); i++)
        {
            mSelectedUnits[i]->positionOrder(NWorld::getPointerPositionView());
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
    NWorld::getCameraManager().getView().move(dt.asSeconds() * 1000.f * mvt);
}

void GameState::onDeactivate()
{
    mWorld.clear();
}
