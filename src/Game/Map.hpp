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

        void tick(sf::Time dt);

        void addChunk(int cx, int cy);
        void removeChunk(int cx, int cy);

        static sf::Vector2i getChunkSize();
        static sf::Vector2i getTileSize();

        std::size_t getChunkCount() const;

        void setTileId(int cx, int cy, int x, int y, int id);
        void setTileId(int x, int y, int id);

        int getTileId(int cx, int cy, int x, int y);
        int getTileId(int x, int y);

        struct Chunk
        {
            sf::Vector2i coords;
            NLayerComponent layer;
        };

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        Chunk* getChunk(int cx, int cy);

    protected:
        std::vector<Chunk*> mChunks;
};

#endif // MAP_HPP
