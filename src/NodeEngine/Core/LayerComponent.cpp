#include "LayerComponent.hpp"
#include "World.hpp"
#include "../Utils/Math.hpp"
#include "../Utils/Compression.hpp"

NLayerComponent::NLayerComponent()
{
}

void NLayerComponent::create(std::string const& textureName, sf::Vector2i mapSize, sf::Vector2i tileSize, int type, int hexSide)
{
    mTexture = textureName;
    mMapSize = mapSize;
    mTileSize = tileSize;
    mType = type;
    mHexSide = hexSide;

    mTiles.resize(mMapSize.x * mMapSize.y);

    for (int i = 0; i < mMapSize.x; ++i)
    {
        for (int j = 0; j < mMapSize.y; ++j)
        {
            sf::Sprite& tile = mTiles[i + j * mMapSize.x];

            if (isIsometric())
            {
                if (j % 2 == 0)
                {
                    tile.setPosition(i * mTileSize.x, j * mTileSize.y * 0.5f);
                }
                else
                {
                    tile.setPosition((i + 0.5f) * mTileSize.x, j * mTileSize.y * 0.5f);
                }
            }
            else if (isHexagonal())
            {
                int sSize = (std::max(mTileSize.x,mTileSize.y) - mHexSide) /2; // TODO : round to upper if not divisible by 2
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
            else
            {
                tile.setPosition(i * mTileSize.x, j * mTileSize.y);
            }
        }
    }

    sf::Texture& texture = NWorld::getResources().getTexture(textureName);
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].setTexture(texture);
    }
}

void NLayerComponent::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform *= getFinalTransform();
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < mMapSize.x; coords.x++)
    {
        for (coords.y = 0; coords.y < mMapSize.y; coords.y++)
        {
            target.draw(mTiles[coords.x + coords.y * mMapSize.x],states);
        }
    }
}

sf::FloatRect NLayerComponent::getBounds() const
{
    return getFinalTransform().transformRect(sf::FloatRect(0,0,mTileSize.x * mMapSize.x,mTileSize.y * mMapSize.y));
}

bool NLayerComponent::contains(NVector const& position) const
{
    return getBounds().contains(NVector::NToSFML2F(position));
}

int NLayerComponent::getType() const
{
    return mType;
}

bool NLayerComponent::isOrthogonal() const
{
    return (mType == NLayerComponent::Orthogonal);
}

bool NLayerComponent::isIsometric() const
{
    return (mType == NLayerComponent::Isometric);
}

bool NLayerComponent::isHexagonal() const
{
    return (mType == NLayerComponent::Hexagonal);
}

void NLayerComponent::setTileId(sf::Vector2i const& coords, int id)
{
    if (coords.x >= 0 && coords.x < mMapSize.x && coords.y >= 0 && coords.y < mMapSize.y)
    {
        mTiles[coords.x + coords.y * mMapSize.x].setTextureRect(idToRect(id));
    }
}

int NLayerComponent::getTileId(sf::Vector2i const& coords)
{
    if (coords.x >= 0 && coords.x < mMapSize.x && coords.y >= 0 && coords.y < mMapSize.y)
    {
        return rectToId(mTiles[coords.x + coords.y * mMapSize.x].getTextureRect());
    }
    return 0;
}

sf::IntRect NLayerComponent::idToRect(int id)
{
    if (mTexture != "")
    {
        int tilePerLine = NWorld::getResources().getTexture(mTexture).getSize().x / mTileSize.x;
        int x = id % tilePerLine;
        int y = id / tilePerLine;
        return sf::IntRect(x * mTileSize.x,y * mTileSize.y, mTileSize.x, mTileSize.y);
    }
    return sf::IntRect();
}

int NLayerComponent::rectToId(sf::IntRect const& rect)
{
    if (mTexture != "")
    {
        int x = rect.left / mTileSize.x;
        int y = rect.top / mTileSize.y;
        int tilePerLine = NWorld::getResources().getTexture(mTexture).getSize().x / mTileSize.x;
        int id = x + y * tilePerLine;
        return id;
    }
    return 0;
}

void NLayerComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    pugi::xml_attribute texture = n.attribute("texture");
    if (texture)
    {
        mTexture = texture.value();
    }
    mMapSize = NVector::NToSFML2I(NString::toVector(n.attribute("msize").value()));
    mTileSize = NVector::NToSFML2I(NString::toVector(n.attribute("tsize").value()));
    mType = n.attribute("type").as_int();
    mHexSide = n.attribute("hexside").as_int();
    setPosition(NString::toVector(n.attribute("pos").value()));
    setScale(NString::toVector(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());

    create(mTexture,mMapSize,mTileSize,mType,mHexSide);

    // Load Tile Data
    std::string data = n.attribute("data").value();
    sf::Vector2i coords;

    std::stringstream ss;
    ss << data;
    ss >> data;

    if (!NCompression::decompress(data))
    {
        return;
    }
    std::vector<unsigned char> byteVector; // hold decompressed data as bytes
    byteVector.reserve(mMapSize.x * mMapSize.y * 4); // number of tiles * 4 bytes = 32bits/tile

    for (std::string::iterator i = data.begin(); i != data.end(); ++i)
    {
        byteVector.push_back(*i);
    }
    for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
    {
        int id = byteVector[i] | byteVector[i+1] << 8 | byteVector[i+2] << 16 | byteVector[i+3] << 24;
        setTileId(coords,id);

        coords.x = (coords.x + 1) % mMapSize.x;
        if (coords.x == 0)
        {
            coords.y++;
        }
    }
}

void NLayerComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    if (mTexture != "")
    {
        n.append_attribute("texture") = mTexture.c_str();
    }
    n.append_attribute("msize") = NString::toString(mMapSize).c_str();
    n.append_attribute("tsize") = NString::toString(mTileSize).c_str();
    n.append_attribute("type") = mType;
    n.append_attribute("hexside") = mHexSide;
    n.append_attribute("pos") = NString::toString(getPosition()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();

    // Save Tile Data
    std::string data;
    data.reserve(mMapSize.x * mMapSize.y * 4);
    sf::Vector2i coords;
    for (coords.y = 0; coords.y < mMapSize.y; coords.y++)
    {
        for (coords.x = 0; coords.x < mMapSize.x; coords.x++)
        {
            const int id = getTileId(coords);
            data.push_back((char)(id));
            data.push_back((char)(id >> 8));
            data.push_back((char)(id >> 16));
            data.push_back((char)(id >> 24));
        }
    }
    if (NCompression::compress(data))
    {
        n.append_attribute("data") = data.c_str();
    }
}
