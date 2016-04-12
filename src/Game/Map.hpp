#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/LayerComponent.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

enum Tile
{
    None = 0,
    Dirt,
    Path,
    Water,
};

class Map : public NActor
{
    public:
        typedef std::shared_ptr<Map> Ptr;

    public:
        Map();

        sf::Vector2i getChunkSize() const;
        sf::Vector2i getTileSize() const;

        void setTileId(int x, int y, int id);
        void setRoadId(int x, int y, int id);
        int getTileId(int x, int y);
        int getRoadId(int x, int y);

        struct Chunk
        {
            sf::Vector2i coords;
            NLayerComponent map;
            NLayerComponent road;
        };

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    private:
        std::vector<Chunk*> mChunks;
};

#endif // MAP_HPP
