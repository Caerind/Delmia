#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"
#include "../NodeEngine/Core/LayerComponent.hpp"

enum Tile
{
    None = 0,
    Dirt,
    Path,
    Water,
};

class Chunk
{
    public:
        typedef std::shared_ptr<Chunk> Ptr;

    public:
        Chunk(sf::Vector2i coords = sf::Vector2i(0,0));

        bool loadFromFile();
        void generate();

        void saveToFile();

        static sf::Vector2i getChunkSize();
        static sf::Vector2i getTileSize();
        static std::string getFilename();

        sf::FloatRect getBounds() const;

        void setTileId(int x, int y, int id);
        int getTileId(int x, int y) const;

        bool loadFromCode(std::string const& code);
        std::string getCode() const;

        sf::Vector2i getCoords() const;

    protected:
        sf::Vector2i mCoords;
        NLayerComponent mLayer;
};

#endif // CHUNK_HPP
