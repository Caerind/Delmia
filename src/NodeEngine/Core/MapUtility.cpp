#include "MapUtility.hpp"

#include <cmath>
#include "../Utils/Math.hpp"

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
    sf::Vector2f mc = {1.f * floor(pos.x / s.x), 1.f * floor(pos.y / s.y)};
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

} // namespace NMapUtility
