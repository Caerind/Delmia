#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/LayerComponent.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

class Map : public NActor
{
    public:
        typedef std::shared_ptr<Map> Ptr;

    public:
        Map();

        void setTileId(int x, int y, int id);
        void setRoadId(int x, int y, int id);
        int getTileId(int x, int y);
        int getRoadId(int x, int y);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    private:
        NLayerComponent mMapComponent;
        NLayerComponent mRoadComponent;
};

#endif // MAP_HPP
