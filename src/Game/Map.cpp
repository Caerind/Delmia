#include "Map.hpp"
#include "../Client/Client.hpp"

std::string Map::gFilename = "assets/data/";

Map::Map()
{
}

Map::~Map()
{
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        removeChunk(mChunks[i]->getCoords());
    }
}

void Map::setFilename(std::string const& filename)
{
    gFilename = filename;
}

std::string Map::getFilename()
{
    return gFilename;
}

void Map::addChunk(sf::Vector2i const& coords)
{
    if (!contains(coords))
    {
        mChunks.push_back(new NIsometric::NLayerComponent("iso",coords));
        if (Client::isOnline())
        {

        }
        else
        {
            if (!load(mChunks.back()))
            {
                generate(mChunks.back());
            }
        }
        attachComponent(mChunks.back());
    }
}

void Map::removeChunk(sf::Vector2i const& coords)
{
    for (std::size_t i = 0; i < mChunks.size();)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            save(mChunks[i]);
            detachComponent(mChunks[i]);
            delete mChunks[i];
            mChunks.erase(mChunks.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void Map::tick(sf::Time dt)
{
    removeUselessChunks();
    addUsefullChunks();
}

bool Map::load(NIsometric::NLayerComponent* chunk)
{
    std::ifstream file(getFilename() + "chunks.map");
    if (file)
    {
        sf::Vector2i coords;
        std::string code;
        while (!file.eof())
        {
            file >> coords.x >> coords.y >> code;
            if (coords == chunk->getCoords())
            {
                return chunk->loadFromCode(code);
            }
        }
    }
    file.close();
    return false;
}

void Map::generate(NIsometric::NLayerComponent* chunk)
{
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < chunk->getLayerSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < chunk->getLayerSize().y; coords.y++)
        {
            chunk->setTileId(coords,1);

            if (coords.y + coords.x == 5)
            {
                chunk->setTileId(coords,2);
            }

            if (coords.y + coords.x == 10)
            {
                chunk->setTileId(coords,3);
            }
        }
    }
}

void Map::save(NIsometric::NLayerComponent* chunk)
{
    std::string code = chunk->getCode();
    if (code != "")
    {
        std::map<std::string,std::string> lines;
        {
            std::ifstream file(getFilename() + "chunks.map");
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
        lines[std::to_string(chunk->getCoords().x) + " " + std::to_string(chunk->getCoords().y)] = code;
        std::ofstream file(getFilename() + "chunks.map");
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

void Map::removeUselessChunks()
{
    std::vector<sf::Vector2i> u = determineUsefullChunks();
    std::vector<sf::Vector2i> r;

    for (std::size_t i = 0; i < mChunks.size();i++)
    {
        bool found = false;
        for (std::size_t j = 0; j < u.size(); j++)
        {
            if (u[j] == mChunks[i]->getCoords())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            r.push_back(mChunks[i]->getCoords());
        }
    }
    for (std::size_t i = 0; i < r.size(); i++)
    {
        removeChunk(r[i]);
    }
}

void Map::addUsefullChunks()
{
    std::vector<sf::Vector2i> u = determineUsefullChunks();
    std::vector<sf::Vector2i> a;

    for (std::size_t i = 0; i < u.size(); i++)
    {
        bool found = false;
        for (std::size_t j = 0; j < mChunks.size(); j++)
        {
            if (u[i] == mChunks[j]->getCoords())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            a.push_back(u[i]);
        }
    }

    for (std::size_t i = 0; i < a.size(); i++)
    {
        addChunk(a[i]);
    }
}

std::vector<sf::Vector2i> Map::determineUsefullChunks()
{
    std::vector<sf::Vector2i> usefull;

    sf::View& view = NWorld::getActiveView();
    sf::Vector2i begin = NIsometric::worldToChunk(view.getCenter() - view.getSize() * 0.5f) - sf::Vector2i(1,1);
    sf::Vector2i end = NIsometric::worldToChunk(view.getCenter() + view.getSize() * 0.5f) + sf::Vector2i(1,1);

    sf::Vector2i coords;
    for (coords.x = begin.x; coords.x <= end.x; coords.x++)
    {
        for (coords.y = begin.y; coords.y <= end.y; coords.y++)
        {
            usefull.push_back(coords);
        }
    }

    return usefull;
}
