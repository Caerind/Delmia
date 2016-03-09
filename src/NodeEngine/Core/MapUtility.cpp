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
    return sf::Vector2i(spos.x/32,spos.y/32); // TODO : Fix negative
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
    // All explanations of this algorithm should follow the drawing
    // Path of this drawing : ...
    // TODO : Add file online

    // Parameters
    sf::Vector2f t = sf::Vector2f(150.f,300.f);
    float h = 150.f;

    // Data
    float d, l, a;
    int type;
    sf::Vector2i r;
    sf::Vector2f rel;

    // X > Y
    if (t.x > t.y)
    {
        d = (t.x - h) * 0.5f;           // Side length (unparralel sides), on x axe
        l = h + d;                      // d + h
        r.x = pos.x / l;                // pos of the rect with l width
        rel.x = pos.x - l * r.x;        // pos relative in the rect on x axe
        if (rel.x > d)                  // last part of the rect
        {
            if (r.x % 2 == 0)
            {
                return sf::Vector2i(r.x, floor(pos.y / t.y));
            }
            else
            {
                return sf::Vector2i(r.x, floor(pos.y - t.y * 0.5f)/t.y);
            }
        }
        else                            // first part of the rect
        {
            r.y = floor((pos.y * 2.f) / t.y); // pos of the rect
            type = (r.y + (r.x % 2)) % 2;  // type
            a = std::atan2(d, t.y * 0.5f); // a en rad

            if (type == 0 && std::atan2(d - rel.x, rel.y * 0.5f) > a)
            {
                r.x--;
                r.y--;
            }
            else if (type == 1 && std::atan2(- rel.x, rel.y) > -a)
            {
                r.x--;
            }
            else if (type == 1 && std::atan2(- rel.x, rel.y) <= -a)
            {
                r.y--;
            }
            return sf::Vector2i(r.x, floor(r.y / 2));
        }
    }
    else // Y > X
    {
        d = (t.y - h) * 0.5f;           // Side length (unparralel sides), on y axe
        l = h + d;                      // d + h
        r.y = pos.y / l;                // pos of the rect with l height
        rel.y = pos.y - l * r.y;        // pos relative in the rect on y axe
        if (rel.y > d)                  // last part of the rect
        {
            if (r.y % 2 == 0)
            {
                return sf::Vector2i(floor(pos.x / t.x), r.y);
            }
            else
            {
                return sf::Vector2i(floor(pos.x - t.x * 0.5f)/t.x, r.y);
            }
        }
        else                            // first part of the rect
        {
            r.x = floor((pos.x * 2.f) / t.x); // pos of the rect
            type = (r.x + (r.y % 2)) % 2;  // type
            a = std::atan2(d, t.x * 0.5f); // a en rad

            if (type == 0 && std::atan2(d - rel.y, rel.x * 0.5f) > a)
            {
                r.x--;
                r.y--;
            }
            else if (type == 1 && std::atan2(- rel.y, rel.x) > -a)
            {
                r.y--;
            }
            else if (type == 1 && std::atan2(- rel.y, rel.x) <= -a)
            {
                r.x--;
            }
            return sf::Vector2i(floor(r.x / 2), r.y);
        }
    }

    return NVector::NToSFML2I(pos);
}

NVector coordsToWorld(sf::Vector2i const& coords)
{
    return NVector::SFML2FToN(sf::Vector2f());
}

} // namespace Hexagonal

} // namespace NMapUtility
