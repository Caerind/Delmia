#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/Isometric.hpp"

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
        ~Map();

        void tick(sf::Time dt);

        void addChunk(int cx, int cy);
        void removeChunk(int cx, int cy);

        std::size_t getChunkCount() const;

        void setTileId(int cx, int cy, int x, int y, int id);
        void setTileId(int x, int y, int id);

        int getTileId(int cx, int cy, int x, int y);
        int getTileId(int x, int y);

        void generate(NIsometric::NLayerComponent* chunk);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

        void removeUselessChunks();
        void addUsefullChunks();

        bool contains(sf::Vector2i const& coords) const;

    protected:
        std::vector<sf::Vector2i> determineUsefullChunks();

    protected:
        std::vector<NIsometric::NLayerComponent*> mChunks;
};

#endif // MAP_HPP
