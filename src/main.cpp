#include "NodeEngine/Application/Application.hpp"
#include "NodeEngine/Core/World.hpp"
#include "NodeEngine/Core/MapUtility.hpp"
#include "NodeEngine/Core/SpriteComponent.hpp"
#include "NodeEngine/Core/LayerComponent.hpp"

class Map : public NActor
{
    public:
        typedef std::shared_ptr<Map> Ptr;

        Map()
        {
            mMapComponent.create("iso",{40,80},{256,128},NLayerComponent::Isometric);
            mRoadComponent.create("iso",{40,80},{256,128},NLayerComponent::Isometric);
            mMapComponent.setPosition(0,0,-2);
            mRoadComponent.setPosition(0,0,-1);
            sf::Vector2i coords;
            for (coords.x = 0; coords.x < 40; coords.x++)
            {
                for (coords.y = 0; coords.y < 80; coords.y++)
                {
                    if (coords.y > 10 && coords.x > 10)
                    {
                        mMapComponent.setTileId(coords,3);
                    }
                    else
                    {
                        mMapComponent.setTileId(coords,1);
                    }
                    mRoadComponent.setTileId(coords,0);
                }
            }
        }

        void setTileId(sf::Vector2i coords, int id)
        {
            mMapComponent.setTileId(coords,id);
        }

        void setRoadId(sf::Vector2i coords, int id)
        {
            mRoadComponent.setTileId(coords,id);
        }

        int getTileId(sf::Vector2i coords)
        {
            return mMapComponent.getTileId(coords);
        }

        int getRoadId(sf::Vector2i coords)
        {
            return mRoadComponent.getTileId(coords);
        }

    private:
        NLayerComponent mMapComponent;
        NLayerComponent mRoadComponent;
};

class Entity : public NActor
{
    public:
        typedef std::shared_ptr<Entity> Ptr;

        Entity() {}
};

class Building : public Entity
{
    public:
        typedef std::shared_ptr<Building> Ptr;

        Building()
        {
        }

        void setCoords(sf::Vector2i coords)
        {
            mCoords = coords;
            for (std::size_t i = 0; i < mSprites.size(); i++)
            {
                mSprites[i].second.setPosition(NMapUtility::Isometric::coordsToWorld(mCoords + mSprites[i].first));
            }
        }

        void addSprite(sf::Vector2i coords, sf::IntRect rect)
        {
            mSprites.emplace_back();
            mSprites.back().first = coords;
            mSprites.back().second.setTexture(NWorld::getResources().getTexture("building"));
            mSprites.back().second.setTextureRect(rect);
            mSprites.back().second.setOrigin(128,192);
            mSprites.back().second.setPosition(NMapUtility::Isometric::coordsToWorld(mCoords + coords));
        }

        sf::Vector2i mCoords;
        std::vector<std::pair<sf::Vector2i,NSpriteComponent>> mSprites;
};

class Unit : public Entity
{
    public:
        typedef std::shared_ptr<Unit> Ptr;

        Unit()
        {
            attachComponent(&mSprite);

            mSprite.setTexture(NWorld::getResources().getTexture("unit"));
            mSprite.setOrigin(64,100);
        }

        NSpriteComponent mSprite;
};

class EState : public ah::State
{
    public:
        EState(ah::StateManager& manager) : ah::State(manager)
        {
            mMap = NWorld::createActor<Map>();

            auto house = NWorld::createActor<Building>();
            house->setCoords(sf::Vector2i(1,1));
            house->addSprite(sf::Vector2i(0,0),sf::IntRect(0,0,256,256));
            mMap->setRoadId(sf::Vector2i(2,2),2);

            auto gold = NWorld::createActor<Building>();
            gold->setCoords(sf::Vector2i(2,6));
            gold->addSprite(sf::Vector2i(0,0),sf::IntRect(256,0,256,256));

            auto forest = NWorld::createActor<Building>();
            forest->setCoords(sf::Vector2i(4,6));
            forest->addSprite(sf::Vector2i(0,0),sf::IntRect(512,0,256,256));

            auto fish = NWorld::createActor<Building>();
            fish->setCoords(sf::Vector2i(13,13));
            fish->addSprite(sf::Vector2i(0,0),sf::IntRect(768,0,256,256));

            auto unit1 = NWorld::createActor<Unit>();
            unit1->setPosition(430,180);

            auto unit2 = NWorld::createActor<Unit>();
            unit2->setPosition(580,375);

            auto unit3 = NWorld::createActor<Unit>();
            unit3->setPosition(620,190);
        }

        bool handleEvent(sf::Event const& event)
        {
            NWorld::addEvent(event);

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i c = NMapUtility::Hexagonal::worldToCoords(NWorld::getPointerPositionView());
                mMap->setTileId(c,1);
            }

            return true;
        }

        bool update(sf::Time dt)
        {
            NWorld::tick(dt);
            NWorld::update();

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
            NWorld::getCameraManager().getView().move(dt.asSeconds() * 400.f * mvt);

            NVector m = NWorld::getPointerPositionView();
            NVector t = NVector::SFML2IToN(NMapUtility::Isometric::worldToCoords(m));

            NWorld::getWindow().setDebugInfo("mouse",NString::toString(m));
            NWorld::getWindow().setDebugInfo("tile",NString::toString(t));

            return true;
        }

        void render(sf::RenderTarget& target, sf::RenderStates states)
        {
            NWorld::render(target);
        }

    private:
        Map::Ptr mMap;
};

int main()
{
    ah::Application::getResources().loadTexture("iso","assets/textures/iso.png");
    ah::Application::getResources().loadTexture("building","assets/textures/building.png");
    ah::Application::getResources().loadTexture("unit","assets/textures/unit.png");
    ah::Application::getResources().loadFont("coolveticca","assets/fonts/coolveticca.ttf");

    ah::Application::getWindow().create(sf::VideoMode(800,600),"NodeEngine",sf::Style::Close);
    ah::Application::getWindow().showDebugInfo(true);
    ah::Application::getWindow().setDebugInfoFont(&ah::Application::getResources().getFont("coolveticca"));
    ah::Application::getWindow().setDebugInfoColor(sf::Color::White);

    ah::Application::getStates().registerState<EState>();
    ah::Application::getStates().pushState<EState>();

    ah::Application::run();

    return 0;
}
