#ifndef MAP_HPP
#define MAP_HPP

#include "../NodeEngine/Core/Isometric.hpp"

class Map : public NIsometric::NMap
{
    public:
        Map();
        ~Map();

        void addChunk(sf::Vector2i const& coords);

        void tick(sf::Time dt);

        void generate(NIsometric::NLayerComponent* chunk);
        void removeUselessChunks();
        void addUsefullChunks();
        std::vector<sf::Vector2i> determineUsefullChunks();
};

#endif // MAP_HPP
