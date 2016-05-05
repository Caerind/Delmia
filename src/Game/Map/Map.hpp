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

        static sf::Vector2i worldToChunk(sf::Vector2f const& pos);
        static sf::Vector2i globalToChunk(sf::Vector2i const& pos);
        static sf::Vector2i globalToRelative(sf::Vector2i const& pos);

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

        void removeUselessChunks();
        void addUsefullChunks();

        bool contains(sf::Vector2i const& coords) const;

    protected:
        std::vector<sf::Vector2i> determineUsefullChunks();

    protected:
        std::vector<Chunk::Ptr> mChunks;
};

#endif // MAP_HPP
