#include "Hexagonal.hpp"

#include "../Utils/Container.hpp"
#include "../Utils/Math.hpp"
#include "World.hpp"

namespace NHexagonal
{

sf::Vector2i gTileSize = sf::Vector2i(1,1);
sf::Vector2i gLayerSize = sf::Vector2i(1,1);

NLayerComponent::NLayerComponent(sf::Vector2i coords)
{
}

NLayerComponent::NLayerComponent(std::string const& textureName, sf::Vector2i layerSize, sf::Vector2i tileSize, int hexSide, sf::Vector2i coords)
{
    setPosition(mCoords.x * layerSize.x * tileSize.x, mCoords.y * layerSize.y * tileSize.y);
    create(textureName,layerSize,tileSize,hexSide);
}

void NLayerComponent::create(std::string const& textureName, sf::Vector2i layerSize, sf::Vector2i tileSize, int hexSide)
{
    mTexture = textureName;
    mLayerSize = layerSize;
    mTileSize = tileSize;
    mHexSide = hexSide;

    mTiles.resize(mLayerSize.x * mLayerSize.y);

    for (int i = 0; i < mLayerSize.x; ++i)
    {
        for (int j = 0; j < mLayerSize.y; ++j)
        {
            sf::Sprite& tile = mTiles[i + j * mLayerSize.x];
            int sSize = (std::max(mTileSize.x,mTileSize.y) - mHexSide) /2;
            if (mTileSize.x > mTileSize.y)
            {
                if (j % 2 == 0)
                {
                    tile.setPosition(i * (mTileSize.x + mHexSide), j * mTileSize.y * 0.5f);
                }
                else
                {
                    tile.setPosition(i * (mTileSize.x + mHexSide) + sSize + mHexSide, j * mTileSize.y * 0.5f);
                }
            }
            else
            {
                if (i % 2 == 0)
                {
                    tile.setPosition(i * mTileSize.x * 0.5f, j * (mTileSize.y + mHexSide));
                }
                else
                {
                    tile.setPosition(i * mTileSize.x * 0.5f, j * (mTileSize.y + mHexSide) + sSize + mHexSide);
                }
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
    return getFinalTransform().transformRect(sf::FloatRect(0, 0, mTileSize.x * mLayerSize.x, mTileSize.y * mLayerSize.y));
}

void NLayerComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    pugi::xml_attribute texture = n.attribute("texture");
    if (texture)
    {
        mTexture = texture.value();
    }
    mLayerSize = NString::toVector2i(n.attribute("lsize").value());
    mTileSize = NString::toVector2i(n.attribute("tsize").value());
    mHexSide = n.attribute("hexside").as_int();
    setPosition(NString::toVector2f(n.attribute("pos").value()));
    setOrigin(NString::toVector2f(n.attribute("ori").value()));
    setScale(NString::toVector2f(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());

    create(mTexture,mLayerSize,mTileSize,mHexSide);

    if (!loadFromCode(n.attribute("data").value()))
    {
        std::cout << "Layer loading problem" << std::endl;
    }
}

void NLayerComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    if (mTexture != "")
    {
        n.append_attribute("texture") = mTexture.c_str();
    }
    n.append_attribute("lsize") = NString::toString(mLayerSize).c_str();
    n.append_attribute("tsize") = NString::toString(mTileSize).c_str();
    n.append_attribute("pos") = NString::toString(getPosition()).c_str();
    n.append_attribute("ori") = NString::toString(getOrigin()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();

    std::string data = getCode();
    if (data != "")
    {
        n.append_attribute("data") = data.c_str();
    }
}


sf::Vector2i worldToCoords(sf::Vector2f const& pos)
{
    // TODO : Hexa - worldToCoords
    return sf::Vector2i();
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
    // TODO : Hexa - coordsToWorld
    return sf::Vector2f();
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
    // TODO : Hexa - Neighboors
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

} // namespace NHexagonal