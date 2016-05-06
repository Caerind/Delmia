#include "SpriteComponent.hpp"
#include "World.hpp"

NSpriteComponent::NSpriteComponent() : mTexture("")
{
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

sf::IntRect NSpriteComponent::getTextureRect() const
{
    return mSprite.getTextureRect();
}

void NSpriteComponent::setColor(sf::Color const& color)
{
    mSprite.setColor(color);
}

sf::Color NSpriteComponent::getColor() const
{
    return mSprite.getColor();
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

bool NSpriteComponent::contains(sf::Vector2f const& position) const
{
    return getBounds().contains(position);
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
    setPosition(NString::toVector2f(n.attribute("pos").value()));
    setOrigin(NString::toVector2f(n.attribute("ori").value()));
    setScale(NString::toVector2f(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());
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
    n.append_attribute("ori") = NString::toString(getOrigin()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();
}
