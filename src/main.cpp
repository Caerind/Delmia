#include "NodeEngine/Application/Application.hpp"
#include "NodeEngine/Core/World.hpp"
#include "NodeEngine/Core/MapUtility.hpp"
#include "NodeEngine/Core/SpriteComponent.hpp"
#include "NodeEngine/Core/LayerComponent.hpp"
#include "NodeEngine/Utils/Log.hpp"

#include <iostream>

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

        void load(pugi::xml_node& node)
        {
            mMapComponent.load(node,"Map");
            mRoadComponent.load(node,"Road");
        }

        void save(pugi::xml_node& node)
        {
            node.append_attribute("type") = "Map";
            mMapComponent.save(node,"Map");
            mRoadComponent.save(node,"Road");
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
            setCoords(0,0);
        }

        ~Building()
        {
            for (std::size_t i = 0; i < mSprites.size(); i++)
            {
                delete mSprites[i].second;
            }
            mSprites.clear();
        }

        void setCoords(int x, int y)
        {
            mCoords = sf::Vector2i(x,y);
            setPosition(NMapUtility::Isometric::coordsToWorld(mCoords) - NMapUtility::Isometric::coordsToWorld(sf::Vector2i(0,0)));
        }

        sf::Vector2i getCoords()
        {
            return mCoords;
        }

        void addSprite(int x, int y, sf::IntRect rect)
        {
            sf::Vector2i coords = sf::Vector2i(x,y);

            std::pair<sf::Vector2i,NSpriteComponent*> pair;
            pair.first = coords;
            pair.second = new NSpriteComponent();
            pair.second->setTexture("building",rect);
            pair.second->setOrigin(128,192);

            pair.second->setPosition(NMapUtility::Isometric::coordsToWorld(coords));

            attachComponent(pair.second);

            mSprites.push_back(pair);
        }

        void load(pugi::xml_node& node)
        {
            std::size_t size = node.attribute("size").as_uint();
            for (std::size_t i = 0; i < size; i++)
            {
                std::string name = "Sprite" + std::to_string(i);
                pugi::xml_node n = node.child(name.c_str());
                sf::Vector2i coords = NVector::NToSFML2I(NString::toVector(n.attribute("coords").value()));
                sf::IntRect rect = NString::toIntRect(n.attribute("rect").value());
                addSprite(coords.x,coords.y,rect);
            }
        }

        void save(pugi::xml_node& node)
        {
            node.append_attribute("type") = "Building";
            node.append_attribute("size") = mSprites.size();
            for (std::size_t i = 0; i < mSprites.size(); i++)
            {
                std::string name = "Sprite" + std::to_string(i);
                pugi::xml_node n = node.append_child(name.c_str());
                n.append_attribute("coords") = NString::toString(NVector::SFML2IToN(mSprites[i].first)).c_str();
                n.append_attribute("rect") = NString::toString(mSprites[i].second->getTextureRect()).c_str();
            }
        }

        sf::Vector2i mCoords;
        std::vector<std::pair<sf::Vector2i,NSpriteComponent*>> mSprites;
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

        void load(pugi::xml_node& node)
        {
        }

        void save(pugi::xml_node& node)
        {
            node.append_attribute("type") = "Unit";
        }

        NSpriteComponent mSprite;
};

class EState : public ah::State
{
    public:
        EState(ah::StateManager& manager) : ah::State(manager)
        {
            NWorld::registerActor<Map>();
            NWorld::registerActor<Unit>();
            NWorld::registerActor<Building>();

            mMap = NWorld::createActor<Map>();

            mHouse = NWorld::createActor<Building>();
            mHouse->setCoords(2,3);
            mHouse->addSprite(0,0,sf::IntRect(0,0,256,256));
            mHouse->addSprite(0,-1,sf::IntRect(0,0,256,256));
            mHouse->addSprite(1,-2,sf::IntRect(0,0,256,256));
            mHouse->addSprite(1,-1,sf::IntRect(0,0,256,256));
            mHouse->addSprite(0,1,sf::IntRect(0,0,256,256));

            NWorld::save("test.xml");
        }

        bool handleEvent(sf::Event const& event)
        {
            NWorld::addEvent(event);

            sf::Vector2i c = NMapUtility::Isometric::worldToCoords(NWorld::getPointerPositionView());
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                mHouse->addSprite(c.x,c.y,sf::IntRect(0,0,256,256));
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
            {
                //mMap->setTileId(c,1);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                NWorld::createActor<Unit>()->setPosition(NWorld::getPointerPositionView());
            }

            // Zoom
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
            m.x = (int)m.x;
            m.y = (int)m.y;
            NVector t = NVector::SFML2IToN(NMapUtility::Isometric::worldToCoords(m));
            t.x = (int)t.x;
            t.y = (int)t.y;

            NWorld::getWindow().setDebugInfo("mouse",NString::toString(m));
            NWorld::getWindow().setDebugInfo("tile",NString::toString(t));

            NWorld::getWindow().setDebugInfo("actors",std::to_string(NWorld::getActorCount()));
            NWorld::getWindow().setDebugInfo("tickables",std::to_string(NWorld::getTickableCount()));
            NWorld::getWindow().setDebugInfo("renderables",std::to_string(NWorld::getRenderableCount()));

            return true;
        }

        void render(sf::RenderTarget& target, sf::RenderStates states)
        {
            NWorld::render(target);
        }

    private:
        Map::Ptr mMap;
        Building::Ptr mHouse;
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
