#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

#include "Chunk.hpp"

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

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        Chunk* getChunk(int cx, int cy);

    protected:
        std::vector<Chunk*> mChunks;
};

#endif // MAP_HPP
