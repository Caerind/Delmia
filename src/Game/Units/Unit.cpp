#include "Unit.hpp"

#include "../../NodeEngine/Utils/ClockedTask.hpp"

Unit::Unit()
{
    mType = Units::DefaultUnit;

    attachComponent(&mSprite);
    mSprite.setPositionZ(1.f);
    initSprite("unit",{61,121},{30.f,110.f});

    setPosition(0.f,0.f);

    mSpeed = 300.f;
}

Unit::Unit(float x, float y)
{
    mType = Units::DefaultUnit;

    attachComponent(&mSprite);
    mSprite.setPositionZ(1.f);
    initSprite("unit",{61,121},{30.f,110.f});

    attachComponent(&mRoot);
    mRoot.setPositionZ(100.f);
    mRoot.setRadius(3.f);
    mRoot.setColor(sf::Color::Blue);

    mSprite.attachComponent(&mNode);
    mNode.setPositionZ(100.f);

    setPosition(x,y);

    mSpeed = 300.f;
}

std::size_t Unit::getType() const
{
    return mType;
}

void Unit::initSprite(std::string const& texture, sf::Vector2i tileSize, sf::Vector2f origin)
{
    mSprite.setTexture(NWorld::getResources().getTexture(texture));
    mSprite.setOrigin(origin);
    mTileSize = tileSize;
    setDirection(Direction::S);
}

void Unit::setDirection(Direction dir)
{
    mDirection = (dir < Direction::Count) ? dir : dir % Direction::Count;
    mSprite.setTextureRect(sf::IntRect({0, (int)mDirection * mTileSize.y}, mTileSize));
}

void Unit::setDirection(float angle)
{
    if (0.f <= angle && angle < 22.5f)
    {
        setDirection(Direction::E);
    }
    else if (22.5f <= angle && angle < 67.5f)
    {
        setDirection(Direction::SE);
    }
    else if (67.5f <= angle && angle < 112.5f)
    {
        setDirection(Direction::S);
    }
    else if (112.5f <= angle && angle < 157.5f)
    {
        setDirection(Direction::SW);
    }
    else if (157.5f <= angle && angle < 202.5f)
    {
        setDirection(Direction::W);
    }
    else if (202.5f <= angle && angle < 247.5f)
    {
        setDirection(Direction::NW);
    }
    else if (247.5f <= angle && angle < 292.5f)
    {
        setDirection(Direction::N);
    }
    else if (292.5f <= angle && angle < 337.5f)
    {
        setDirection(Direction::NE);
    }
    else if (337.5f <= angle && angle <= 360.f)
    {
        setDirection(Direction::E);
    }
    else
    {
        setDirection(Direction::S);
    }
}

std::size_t Unit::getDirection() const
{
    return mDirection;
}

void Unit::positionOrder(sf::Vector2f const& position)
{
    mPositionOrder = position;
    mPathDone = false;
}

void Unit::tick(sf::Time dt)
{
    if (mPositionOrder != sf::Vector2f())
    {
        if (!mPathDone)
        {
            sf::Vector2i b = NMapUtility::Isometric::worldToCoords(getPosition());
            sf::Vector2i e = NMapUtility::Isometric::worldToCoords(mPositionOrder);
            NClockedTask t([b,e,this]()
            {
                mPath = NMapUtility::pathfinding(NMapUtility::Type::Isometric,b,e);
            });
            std::cout << "Path in : " << t.execute().asSeconds() << "s" << std::endl;
            mPathDone = true;
        }

        sf::Vector2f dest;
        if (mPath.size() > 0)
        {
            dest = NMapUtility::Isometric::coordsToWorld(mPath.front());
        }
        else
        {
            dest = mPositionOrder;
        }

        sf::Vector2f delta = dest - getPosition();
        move(normalizedVector(delta) * dt.asSeconds() * mSpeed);
        setDirection(getPolarAngle(delta));

        if (mPath.size() > 0)
        {
            sf::Vector2f p = NMapUtility::Isometric::coordsToWorld(mPath.front());
            if (getLength(getPosition() - p) < 30.f)
            {
                erase(mPath, 0);
            }
        }

        if (getLength(mPositionOrder - getPosition()) < 30.f)
        {
            mPositionOrder = sf::Vector2f();
            mPathDone = false;
        }
    }
}

void Unit::load(pugi::xml_node& node)
{
}

void Unit::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Unit";
}
