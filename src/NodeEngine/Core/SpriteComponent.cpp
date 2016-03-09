#include "SpriteComponent.hpp"
#include "World.hpp"

NSpriteComponent::NSpriteComponent() : mTexture("")
{
}

void NSpriteComponent::setOrigin(float x, float y, float z)
{
    setOrigin(NVector(x,y,z));
}

void NSpriteComponent::setOrigin(NVector const& origin)
{
    mOrigin = origin;
    mSprite.setOrigin(NVector::NToSFML2F(origin));
}

NVector NSpriteComponent::getOrigin() const
{
    return NVector::SFML2FToN(mSprite.getOrigin());
}

void NSpriteComponent::setTexture(std::string const& textureName, sf::IntRect const& rect)
{
    setTexture(NWorld::getResources().getTexture(textureName),rect);
    mTexture = textureName;
}

void NSpriteComponent::setTexture(sf::Texture& texture, sf::IntRect const& rect)
{
    mSprite.setTexture(texture);
    if (rect != sf::IntRect())
    {
        setTextureRect(rect);
    }
}

void NSpriteComponent::setTextureRect(sf::IntRect const& rect)
{
    mSprite.setTextureRect(rect);
}

void NSpriteComponent::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform *= getFinalTransform();
    target.draw(mSprite,states);
}

sf::FloatRect NSpriteComponent::getBounds() const
{
    sf::FloatRect r = getFinalTransform().transformRect(mSprite.getLocalBounds());
    r.left -= getOrigin().x;
    r.top -= getOrigin().y;
    return r;
}

bool NSpriteComponent::contains(NVector const& position) const
{
    return getBounds().contains(NVector::NToSFML2F(position));
}

void NSpriteComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    pugi::xml_attribute texture = n.attribute("texture");
    if (texture)
    {
        setTexture(texture.value());
        setTextureRect(NString::toIntRect(n.attribute("rect").value()));
    }
    setPosition(NString::toVector(n.attribute("pos").value()));
    setScale(NString::toVector(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());
    setOrigin(NString::toVector(n.attribute("ori").value()));
}

void NSpriteComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    if (mTexture != "")
    {
        n.append_attribute("texture") = mTexture.c_str();
        n.append_attribute("rect") = NString::toString(mSprite.getTextureRect()).c_str();
    }
    n.append_attribute("pos") = NString::toString(getPosition()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();
    n.append_attribute("ori") = NString::toString(mOrigin).c_str();
}
