#include "Isometric.hpp"

#include "../Utils/Container.hpp"
#include "../Utils/Math.hpp"
#include "World.hpp"

namespace NIsometric
{

sf::Vector2i gTileSize = sf::Vector2i(1,1);
sf::Vector2i gLayerSize = sf::Vector2i(1,1);

NLayerComponent::NLayerComponent(sf::Vector2i coords)
{
    mCoords = coords;
}

NLayerComponent::NLayerComponent(std::string const& textureName, sf::Vector2i coords)
{
    mCoords = coords;
    setPosition(mCoords.x * NIsometric::getLayerSize().x * NIsometric::getTileSize().x, mCoords.y * 0.5f * NIsometric::getLayerSize().y * NIsometric::getTileSize().y);
    create(textureName,NIsometric::getLayerSize(),NIsometric::getTileSize());
}

NLayerComponent::NLayerComponent(std::string const& textureName, sf::Vector2i layerSize, sf::Vector2i tileSize, sf::Vector2i coords)
{
    mCoords = coords;
    setPosition(mCoords.x * layerSize.x * tileSize.x, mCoords.y * 0.5f * layerSize.y * tileSize.y);
    create(textureName,layerSize,tileSize);
}

void NLayerComponent::create(std::string const& textureName, sf::Vector2i layerSize, sf::Vector2i tileSize)
{
    mTexture = textureName;
    mLayerSize = layerSize;
    mTileSize = tileSize;

    mTiles.resize(mLayerSize.x * mLayerSize.y);

    for (int i = 0; i < mLayerSize.x; ++i)
    {
        for (int j = 0; j < mLayerSize.y; ++j)
        {
            sf::Sprite& tile = mTiles[i + j * mLayerSize.x];
            if (j % 2 == 0)
            {
                tile.setPosition(i * mTileSize.x, j * mTileSize.y * 0.5f);
            }
            else
            {
                tile.setPosition((i + 0.5f) * mTileSize.x, j * mTileSize.y * 0.5f);
            }
        }
    }

    sf::Texture& texture = NWorld::getResources().getTexture(textureName);
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].setTexture(texture);
    }
}

sf::FloatRect NLayerComponent::getBounds() const
{
    return getFinalTransform().transformRect(sf::FloatRect(0, 0, mTileSize.x * mLayerSize.x, mTileSize.y * mLayerSize.y * 0.5f));
}


sf::Vector2i worldToCoords(sf::Vector2f const& pos)
{
    sf::Vector2f s = {getTileSize().x * 0.5f, getTileSize().y * 0.5f};
    sf::Vector2f mc = {(float)floor(pos.x / s.x), (float)floor(pos.y / s.y)};
    sf::Vector2f p = pos;
    p -= {mc.x * s.x, mc.y * s.y};
    if (((int)mc.x + (int)mc.y) % 2 == 0)
    {
        if (NMath::atan2(s.y - p.y,p.x) > 30.f)
        {
            mc.x--;
            mc.y--;
        }
    }
    else
    {
        if (NMath::atan2(-p.y,p.x) > -30.f)
        {
            mc.y--;
        }
        else
        {
            mc.x--;
        }
    }
    return {(int)floor(mc.x * 0.5f),(int)mc.y};
}

sf::Vector2i worldToChunk(sf::Vector2f const& pos)
{
    return coordsToChunk(worldToCoords(pos));
}

sf::Vector2i worldToRelative(sf::Vector2f const& pos)
{
    return coordsToRelative(worldToCoords(pos));
}

sf::Vector2f coordsToWorld(sf::Vector2i const& coords)
{
    sf::Vector2f ret;
    ret.y = coords.y * getTileSize().y * 0.5f + getTileSize().y * 0.5f;
    if (coords.y % 2 == 0)
    {
        ret.x = coords.x * getTileSize().x + getTileSize().x * 0.5f;
    }
    else
    {
        ret.x = coords.x * getTileSize().x + getTileSize().x;
    }
    return ret;
}

sf::Vector2i coordsToChunk(sf::Vector2i const& coords)
{
    sf::Vector2i c;
    if (getLayerSize() != sf::Vector2i(0,0))
    {
        c.x = coords.x / getLayerSize().x;
        c.y = coords.y / getLayerSize().y;
        if (coords.x < 0)
        {
            c.x--;
        }
        if (coords.y < 0)
        {
            c.y--;
        }
    }
    return c;
}

sf::Vector2i coordsToRelative(sf::Vector2i const& coords)
{
    sf::Vector2i c;
    c.x  = coords.x % getLayerSize().x;
    c.y  = coords.y % getLayerSize().y;
    if (c.x < 0)
    {
        c.x += getLayerSize().x;
    }
    if (c.y < 0)
    {
        c.y += getLayerSize().y;
    }
    return c;
}

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords, bool diag)
{
    std::vector<sf::Vector2i> n;
    if (coords.y % 2 == 0)
    {
        n.push_back({coords.x - 1, coords.y - 1});
        n.push_back({coords.x, coords.y - 1});
        n.push_back({coords.x - 1, coords.y + 1});
        n.push_back({coords.x, coords.y + 1});
    }
    else
    {
        n.push_back({coords.x, coords.y - 1});
        n.push_back({coords.x + 1, coords.y - 1});
        n.push_back({coords.x, coords.y + 1});
        n.push_back({coords.x + 1, coords.y + 1});
    }
    if (diag)
    {
        n.push_back({coords.x, coords.y - 1});
        n.push_back({coords.x + 1, coords.y});
        n.push_back({coords.x, coords.y + 1});
        n.push_back({coords.x - 1, coords.y});
    }
    return n;
}

namespace priv
{
    struct Node
    {
        Node(sf::Vector2i c, sf::Vector2i p) { coords = c; parent = p; }
        sf::Vector2i coords;
        sf::Vector2i parent;
    };
} // namespace priv

std::vector<sf::Vector2i> pathfinding(sf::Vector2i const& begin, sf::Vector2i const& end)
{
    std::vector<priv::Node> container;
    add(container, priv::Node(begin,begin));
    std::size_t test = 0;
    bool reached = false;
    while (test < 50 && !reached)
    {
        std::vector<priv::Node> temp;
        for (std::size_t i = 0; i < container.size(); i++)
        {
            std::vector<sf::Vector2i> n = getNeighboors(container[i].coords);
            for (std::size_t j = 0; j < n.size(); j++)
            {
                bool found = false;
                for (std::size_t k = 0; k < container.size(); k++)
                {
                    if (container[k].coords == n[j])
                    {
                        found = true;
                    }
                }
                for (std::size_t k = 0; k < temp.size(); k++)
                {
                    if (temp[k].coords == n[j])
                    {
                        found = true;
                    }
                }
                if (!found)
                {
                    add(temp, priv::Node(n[j], container[i].coords));
                    if (n[j] == end)
                    {
                        reached = true;
                    }
                }
            }
        }
        append(container, temp);
        test++;
    }

    std::vector<sf::Vector2i> path;
    if (reached)
    {
        sf::Vector2i pos = end;
        reached = false;
        while (!reached)
        {
            bool handled = false;
            for (std::size_t i = 0; i < container.size(); i++)
            {
                if (container[i].coords == pos && !handled)
                {
                    add(path, pos);
                    if (container[i].parent == container[i].coords)
                    {
                        reached = true;
                    }
                    pos = container[i].parent;
                    handled = true;
                }
            }
        }
        std::reverse(path.begin(),path.end());
    }
    return path;
}

void setTileSize(sf::Vector2i const& tileSize)
{
    gTileSize = tileSize;
}

void setLayerSize(sf::Vector2i const& layerSize)
{
    gLayerSize = layerSize;
}

sf::Vector2i getTileSize()
{
    return gTileSize;
}

sf::Vector2i getLayerSize()
{
    return gLayerSize;
}

} // namespace NIsometric
