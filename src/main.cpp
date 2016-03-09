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
            mMapComponent.create("iso",{20,40},{256,128},NLayerComponent::Isometric);
            sf::Vector2i coords;
            for (coords.x = 0; coords.x < 30; coords.x++)
            {
                for (coords.y = 0; coords.y < 50; coords.y++)
                {
                    mMapComponent.setTileId(coords,0);
                }
            }
        }

        void setTileId(sf::Vector2i coords, int id)
        {
            mMapComponent.setTileId(coords,id);
        }

    private:
        NLayerComponent mMapComponent;
};

class Building : public NActor
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

class EState : public ah::State
{
    public:
        EState(ah::StateManager& manager) : ah::State(manager)
        {
            mMap = NWorld::createActor<Map>();
            mBui = NWorld::createActor<Building>();
            mBui->setCoords(sf::Vector2i(1,1));
            mBui->addSprite(sf::Vector2i(0,0),sf::IntRect(0,0,256,256));
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
            mvt = dt.asSeconds() * 400.f * mvt;
            NWorld::getCameraManager().getView().move(mvt);

            return true;
        }

        void render(sf::RenderTarget& target, sf::RenderStates states)
        {
            NWorld::render(target);
        }

    private:
        Map::Ptr mMap;
        Building::Ptr mBui;
};

int main()
{
    ah::Application::getResources().loadTexture("iso","assets/textures/iso.png");
    ah::Application::getResources().loadTexture("building","assets/textures/building.png");
    ah::Application::getWindow().create(sf::VideoMode(800,600),"NodeEngine",sf::Style::Close);

    ah::Application::getStates().registerState<EState>();
    ah::Application::getStates().pushState<EState>();

    ah::Application::run();

    return 0;
}
