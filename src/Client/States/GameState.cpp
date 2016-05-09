#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mCameraSpeed = 1000.f;

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

    // Left Click Pressed
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !mPlacing)
    {
        mPlacing = true;
        switch (mPlacingType)
        {
            case 0: mPlacement = mWorld.createActor<Hall>(c); break;
            case 1: mPlacement = mWorld.createActor<Market>(c); break;
            case 2: mPlacement = mWorld.createActor<Barrack>(c); break;
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
                case 0: mWorld.createBuilding<Hall>(c); break;
                case 1: mWorld.createBuilding<Market>(c); break;
                case 2: mWorld.createBuilding<Barrack>(c); break;
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
        mPlacingType = (mPlacingType >= 3) ? 0 : mPlacingType + 1;
        if (mPlacing)
        {
            if (mPlacement != nullptr)
            {
                mWorld.removeActor(mPlacement->getId());
            }
            switch (mPlacingType)
            {
                case 0: mPlacement = mWorld.createActor<Hall>(c); break;
                case 1: mPlacement = mWorld.createActor<Market>(c); break;
                case 2: mPlacement = mWorld.createActor<Barrack>(c); break;
                default: mPlacement = nullptr; break;
            }
            if (mPlacement != nullptr)
            {
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
        mPlacement->generate(coords);
        mPlacement->setPositionZ(100000.f);
        bool collide;
        switch (mPlacingType)
        {
            case 0: collide = !mWorld.buildingPlacing<Hall>(coords); break;
            case 1: collide = !mWorld.buildingPlacing<Market>(coords); break;
            case 2: collide = !mWorld.buildingPlacing<Barrack>(coords); break;
            default: collide = false; break;
        }
        mPlacement->setColor((collide) ? sf::Color::Red : sf::Color::Green);
    }
}

void GameState::handleUnit(sf::Event const& event)
{
    sf::Vector2f p = NWorld::getPointerPositionView();

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)
    {
        mWorld.createUnit<Unit>(p);
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

    if (mSelectedUnits.size() > 0 && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
    {
        /*if (allies building not built)
            citizens selected go build
        if (allies building built)
            all go depose resources
        if (resource)
            citizens selected go gather
        if (enemy)
            attack and rob
        else
            go
            */


        /*
        sf::Vector2f pos = NWorld::getPointerPositionView();
        if (!mWorld.collide(NIsometric::worldToCoords(pos)))
        {
            for (std::size_t i = 0; i < mSelectedUnits.size(); i++)
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
                mSelectedUnits[i]->positionOrder(pos);
            }
        }
        */
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

void GameState::onDeactivate()
{
    mWorld.clear();
}
