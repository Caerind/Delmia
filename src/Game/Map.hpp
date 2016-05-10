#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/Isometric.hpp"

enum Tile
{
    None = 0,
    Dirt,
    Path,
    Water,
};

class Map : public NIsometric::NMap
{
    public:
        Map();
        ~Map();

        static void setFilename(std::string const& filename);
        static std::string getFilename();

        void addChunk(sf::Vector2i const& coords);
        void removeChunk(sf::Vector2i const& coords);

        void tick(sf::Time dt);

        bool load(NIsometric::NLayerComponent* chunk);
        void save(NIsometric::NLayerComponent* chunk);

        void removeUselessChunks();
        void addUsefullChunks();
        std::vector<sf::Vector2i> determineUsefullChunks();

        void generate(NIsometric::NLayerComponent* chunk);

    protected:
        static std::string gFilename;
};

#endif // MAP_HPP
