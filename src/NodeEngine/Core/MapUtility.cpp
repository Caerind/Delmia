#include "MapUtility.hpp"

#include <cmath>
#include "../Utils/Math.hpp"
#include "../Utils/Container.hpp"

namespace NMapUtility
{

namespace Orthogonal
{

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords, bool diag)
{
    std::vector<sf::Vector2i> n;
	n.push_back({coords.x, coords.y - 1});
	n.push_back({coords.x, coords.y + 1});
	n.push_back({coords.x - 1, coords.y});
	n.push_back({coords.x + 1, coords.y});
	if (diag)
    {
        n.push_back({coords.x + 1, coords.y - 1});
        n.push_back({coords.x + 1, coords.y + 1});
        n.push_back({coords.x - 1, coords.y + 1});
        n.push_back({coords.x - 1, coords.y - 1});
    }
    return n;
}

sf::Vector2i worldToCoords(sf::Vector2f const& pos)
{
    return {(int)pos.x / 32, (int)pos.y / 32};
}

sf::Vector2f coordsToWorld(sf::Vector2i const& coords) // center of the tile
{
    return {coords.x * 32 + 0.5f * 32, coords.y * 32 + 0.5f * 32};
}

} // namespace Orthogonal

namespace Isometric
{

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

sf::Vector2i worldToCoords(sf::Vector2f const& pos)
{
    sf::Vector2f s = {256.f * 0.5f, 128.f * 0.5f}; // HERE YOUR TILE SIZE
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

sf::Vector2f coordsToWorld(sf::Vector2i const& coords)
{
    sf::Vector2f ret;
    ret.y = coords.y * 128 * 0.5f + 128 * 0.5f;
    if (coords.y % 2 == 0)
    {
        ret.x = coords.x * 256 + 256 * 0.5f;
    }
    else
    {
        ret.x = coords.x * 256 + 256;
    }
    return ret;
}

} // namespace Isometric

namespace Hexagonal
{

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> n;
    return n;
}

sf::Vector2i worldToCoords(sf::Vector2f const& pos)
{
    return {};
}

sf::Vector2f coordsToWorld(sf::Vector2i const& coords)
{
    return {};
}

} // namespace Hexagonal

std::vector<sf::Vector2i> pathfinding(Type::MapType type, sf::Vector2i const& begin, sf::Vector2i const& end)
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
            std::vector<sf::Vector2i> n;
            switch (type)
            {
                case Type::Orthogonal: n = Orthogonal::getNeighboors(container[i].coords); break;
                case Type::Isometric: n = Isometric::getNeighboors(container[i].coords); break;
                case Type::Hexagonal: n = Hexagonal::getNeighboors(container[i].coords); break;
                default: break;
            }
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

} // namespace NMapUtility
