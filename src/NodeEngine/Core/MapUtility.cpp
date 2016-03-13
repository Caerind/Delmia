#include "MapUtility.hpp"

namespace NMapUtility
{

namespace Orthogonal
{

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> n;
	n.push_back(sf::Vector2i(coords.x,coords.y-1));
	n.push_back(sf::Vector2i(coords.x,coords.y+1));
	n.push_back(sf::Vector2i(coords.x-1,coords.y));
	n.push_back(sf::Vector2i(coords.x+1,coords.y));
    return n;
}

sf::Vector2i worldToCoords(NVector const& pos)
{
    sf::Vector2f spos = NVector::NToSFML2F(pos);
    return sf::Vector2i(spos.x/32,spos.y/32);
}

NVector coordsToWorld(sf::Vector2i const& coords) // center of the tile
{
    sf::Vector2f ret;
	ret.x = coords.x * 32 + 0.5f * 32;
	ret.y = coords.y * 32 + 0.5f * 32;
    return NVector::SFML2FToN(ret);
}

} // namespace Orthogonal

namespace Isometric
{

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> n;
    if (coords.y % 2 == 0)
    {
        n.push_back(sf::Vector2i(coords.x-1,coords.y-1));
        n.push_back(sf::Vector2i(coords.x,coords.y-1));
        n.push_back(sf::Vector2i(coords.x-1,coords.y+1));
        n.push_back(sf::Vector2i(coords.x,coords.y+1));
    }
    else
    {
        n.push_back(sf::Vector2i(coords.x,coords.y-1));
        n.push_back(sf::Vector2i(coords.x+1,coords.y-1));
        n.push_back(sf::Vector2i(coords.x,coords.y+1));
        n.push_back(sf::Vector2i(coords.x+1,coords.y+1));
    }
    return n;
}

sf::Vector2i worldToCoords(NVector const& pos)
{
    sf::Vector2f spos = NVector::NToSFML2F(pos);
    sf::Vector2f s = {256 * 0.5f, 128 * 0.5f};
    sf::Vector2f mc = sf::Vector2f(floor(spos.x / s.x),floor(spos.y / s.y));
    sf::Vector2f p = spos - sf::Vector2f(mc.x * s.x, mc.y * s.y);
    if (((int)mc.x + (int)mc.y) % 2 == 0)
    {
        if (std::atan2(s.y - p.y,p.x) * 180.f / 3.14152f > 30)
        {
            mc.x--;
            mc.y--;
        }
    }
    else
    {
        if (std::atan2(-p.y,p.x) * 180.f / 3.14152f > -30)
        {
            mc.y--;
        }
        else
        {
            mc.x--;
        }
    }
    return sf::Vector2i(floor(mc.x * 0.5f),mc.y);
}

NVector coordsToWorld(sf::Vector2i const& coords)
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
    return NVector::SFML2FToN(ret);
}

} // namespace Isometric

namespace Hexagonal
{

std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> n;
    return n;
}

sf::Vector2i worldToCoords(NVector const& pos)
{
    return NVector::NToSFML2I(pos);
}

NVector coordsToWorld(sf::Vector2i const& coords)
{
    return NVector::SFML2FToN(sf::Vector2f());
}

} // namespace Hexagonal

} // namespace NMapUtility
