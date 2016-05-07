#include "LayerComponent.hpp"
#include "World.hpp"
#include "../Utils/Math.hpp"
#include "../Utils/Compression.hpp"

NLayerComponent::NLayerComponent(sf::Vector2i coords)
: mCoords(coords)
{
}

NLayerComponent::NLayerComponent(std::string const& textureName, sf::Vector2i layerSize, sf::Vector2i tileSize, sf::Vector2i coords)
: mCoords(coords)
{
    setPosition(mCoords.x * layerSize.x * tileSize.x, mCoords.y * layerSize.y * tileSize.y);
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
            tile.setPosition(i * mTileSize.x, j * mTileSize.y);
        }
    }

    sf::Texture& texture = NWorld::getResources().getTexture(textureName);
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].setTexture(texture);
    }
}

sf::Vector2i NLayerComponent::getCoords() const
{
    return mCoords;
}

bool NLayerComponent::loadFromCode(std::string const& code)
{
    sf::Vector2i coords;

    std::string data;
    std::stringstream ss;
    ss << code;
    ss >> data;

    if (!NCompression::decompress(data))
    {
        return false;
    }

    std::vector<unsigned char> byteVector; // hold decompressed data as bytes
    byteVector.reserve(mLayerSize.x * mLayerSize.y * 4); // number of tiles * 4 bytes = 32bits/tile

    for (std::string::iterator i = data.begin(); i != data.end(); ++i)
    {
        byteVector.push_back(*i);
    }
    for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
    {
        int id = byteVector[i] | byteVector[i+1] << 8 | byteVector[i+2] << 16 | byteVector[i+3] << 24;
        setTileId(coords,id);

        coords.x = (coords.x + 1) % mLayerSize.x;
        if (coords.x == 0)
        {
            coords.y++;
        }
    }

    return (coords.x == 0 && coords.y == mLayerSize.y);
}

std::string NLayerComponent::getCode() const
{
    std::string data;
    data.reserve(mLayerSize.x * mLayerSize.y * 4);
    sf::Vector2i coords;
    for (coords.y = 0; coords.y < mLayerSize.y; coords.y++)
    {
        for (coords.x = 0; coords.x < mLayerSize.x; coords.x++)
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
        return data;
    }
    else
    {
        return "";
    }
}

sf::Vector2i NLayerComponent::getLayerSize() const
{
    return mLayerSize;
}

sf::Vector2i NLayerComponent::getTileSize() const
{
    return mTileSize;
}

void NLayerComponent::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform *= getFinalTransform();
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < mLayerSize.x; coords.x++)
    {
        for (coords.y = 0; coords.y < mLayerSize.y; coords.y++)
        {
            target.draw(mTiles[coords.x + coords.y * mLayerSize.x],states);
        }
    }
}

sf::FloatRect NLayerComponent::getBounds() const
{
    return getFinalTransform().transformRect(sf::FloatRect(0, 0, mTileSize.x * mLayerSize.x, mTileSize.y * mLayerSize.y));
}

void NLayerComponent::setTileId(sf::Vector2i const& coords, int id)
{
    if (0 <= coords.x && coords.x < mLayerSize.x && 0 <= coords.y && coords.y < mLayerSize.y)
    {
        mTiles[coords.x + coords.y * mLayerSize.x].setTextureRect(idToRect(id));
    }
}

int NLayerComponent::getTileId(sf::Vector2i const& coords) const
{
    if (0 <= coords.x && coords.x < mLayerSize.x && 0 <= coords.y && coords.y < mLayerSize.y)
    {
        return rectToId(mTiles.at(coords.x + coords.y * mLayerSize.x).getTextureRect());
    }
    return 0;
}

sf::IntRect NLayerComponent::idToRect(int id) const
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

int NLayerComponent::rectToId(sf::IntRect const& rect) const
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
    mLayerSize = NString::toVector2i(n.attribute("lsize").value());
    mTileSize = NString::toVector2i(n.attribute("tsize").value());
    setPosition(NString::toVector2f(n.attribute("pos").value()));
    setOrigin(NString::toVector2f(n.attribute("ori").value()));
    setScale(NString::toVector2f(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());

    create(mTexture,mLayerSize,mTileSize);

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
