#include "Chunk.hpp"

#include <fstream>
#include <map>

Chunk::Chunk(sf::Vector2i coords)
{
    mCoords = coords;
    mLayer.create("iso",getChunkSize(),getTileSize(),NLayerComponent::Isometric);
    mLayer.setPosition(getChunkSize().x * getTileSize().x * coords.x, 0.5f * getChunkSize().y * getTileSize().y * coords.y, -10.f);
}

bool Chunk::loadFromFile()
{
    std::ifstream file(getFilename());
    if (file)
    {
        sf::Vector2i coords;
        std::string code;
        while (!file.eof())
        {
            file >> coords.x >> coords.y >> code;
            if (coords == mCoords)
            {
                return loadFromCode(code);
            }
        }
    }
    file.close();
    return false;
}

void Chunk::generate()
{
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < getChunkSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < getChunkSize().y; coords.y++)
        {
            mLayer.setTileId(coords,Tile::Dirt);

            if (coords.y + coords.x == 5)
            {
                mLayer.setTileId(coords,Tile::Path);
            }

            if (coords.y + coords.x == 10)
            {
                mLayer.setTileId(coords,Tile::Water);
            }
        }
    }
}

void Chunk::saveToFile()
{
    std::string code = getCode();
    if (code != "")
    {
        std::map<std::string,std::string> lines;
        {
            std::ifstream file(getFilename());
            if (file)
            {
                std::string line;
                while (std::getline(file,line))
                {
                    std::size_t f = line.find_last_of(" ");
                    if (f != std::string::npos)
                    {
                        lines[line.substr(0,f)] = line.substr(f+1);
                    }
                }
            }
            file.close();
        }
        lines[std::to_string(getCoords().x) + " " + std::to_string(getCoords().y)] = code;
        std::ofstream file(getFilename());
        if (file)
        {
            for (auto itr = lines.begin(); itr != lines.end(); itr++)
            {
                file << itr->first << " " << itr->second << std::endl;
            }
        }
        file.close();
    }
}

sf::Vector2i Chunk::getChunkSize()
{
    return sf::Vector2i(16,64);
}

sf::Vector2i Chunk::getTileSize()
{
    return sf::Vector2i(256,128);
}

std::string Chunk::getFilename()
{
    return "assets/data/chunks.map";
}

sf::FloatRect Chunk::getBounds() const
{
    return sf::FloatRect(NVector::NToSFML2F(mLayer.getPosition()),sf::Vector2f(getChunkSize().x * getTileSize().x,getChunkSize().y * getTileSize().y));
}

void Chunk::setTileId(int x, int y, int id)
{
    mLayer.setTileId(sf::Vector2i(x,y),id);
}

int Chunk::getTileId(int x, int y) const
{
    return mLayer.getTileId(sf::Vector2i(x,y));
}

bool Chunk::loadFromCode(std::string const& code)
{
    return mLayer.loadFromCode(code);
}

std::string Chunk::getCode() const
{
    return mLayer.getCode();
}

sf::Vector2i Chunk::getCoords() const
{
    return mCoords;
}
