#include "Unit.hpp"
#include "World.hpp"

#include "../../NodeEngine/Utils/ClockedTask.hpp"

Unit::Unit()
: PlayerOwned(nullptr)
, mThread(&Unit::calculatePath, this)
{
    mType = Units::DefaultUnit;

    attachComponent(&mSprite);
    mSprite.setPositionZ(1.f);
    initSprite("unit",{61,121},{30.f,110.f});
    setPosition(0.f,0.f);

    mSpeed = 300.f;
    mMovingTime = sf::Time::Zero;
}

Unit::Unit(Player* player, sf::Vector2f const& pos)
: PlayerOwned(player)
, mThread(&Unit::calculatePath, this)
{
    mType = Units::Citizen; // TODO : Change

    attachComponent(&mSprite);
    mSprite.setPositionZ(1.f);
    initSprite("unit",{61,121},{30.f,110.f});
    setPosition(pos);

    mSpeed = 300.f;
    mMovingTime = sf::Time::Zero;
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

sf::Vector2i Unit::getCoords() const
{
    return NIsometric::worldToCoords(getPosition());
}

void Unit::setDirection(Direction dir)
{
    mDirection = (dir < Direction::Count) ? dir : dir % Direction::Count;
    updateTextureRect();
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

void Unit::moveToDest(sf::Vector2f const& dest, sf::Time dt)
{
    sf::Vector2f delta = dest - getPosition();
    if (!isZero(delta))
    {
        mMovingTime += dt;
        if (mMovingTime >= sf::seconds(0.8f))
        {
            mMovingTime = sf::Time::Zero;
        }
        move(normalizedVector(delta) * dt.asSeconds() * mSpeed);
        setDirection(getPolarAngle(delta));
    }
}

void Unit::updateTextureRect()
{
    mSprite.setTextureRect(sf::IntRect({mTileSize.x * ((int)(mMovingTime.asSeconds() * 10.f)), (int)mDirection * mTileSize.y}, mTileSize));
}

void Unit::calculatePath()
{
    std::vector<sf::Vector2i> tPath;
    sf::Vector2i b = NIsometric::worldToCoords(getPosition());
    sf::Vector2i e = NIsometric::worldToCoords(mPositionOrder);
    NClockedTask t([b,e,&tPath,this]()
    {
        tPath = NIsometric::pathfinding(b,e,[this](sf::Vector2i const& coords)->bool{return mWorld->collide(coords);});
    });

    sf::Lock lock(mMutex);
    std::cout << "Path in : " << t.execute().asSeconds() << "s" << std::endl;
    mPath = tPath;
    mPathDone = true;
    if (mPath.size() == 0)
    {
        mPathDone = false;
        mPositionOrder = sf::Vector2f();
    }
}

void Unit::onBuildingAdded(sf::Vector2i const& coords)
{
    bool recalculated = false;
    for (std::size_t i = 0; i < mPath.size(); i++)
    {
        if (!recalculated && mPath[i] == coords)
        {
            calculatePath();
            recalculated = true;
        }
    }
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
            calculatePath();
        }

        if (mPath.size() > 0)
        {
            sf::Vector2f p = NIsometric::coordsToWorld(mPath.front());
            moveToDest(p, dt);
            if (getLength(p - getPosition()) < 100.f)
            {
                erase(mPath, 0);
            }
        }
        else
        {
            moveToDest(mPositionOrder, dt);
        }

        if (getLength(mPositionOrder - getPosition()) < 30.f)
        {
            mPositionOrder = sf::Vector2f();
            mPathDone = false;
            mMovingTime = sf::Time::Zero;
            updateTextureRect();
        }
    }
}

void Unit::takeResource(Entity* entity, std::size_t resourceId, int amount)
{
    if (entity != nullptr)
    {
        if (entity->hasResourceAmount(resourceId,amount))
        {
            addResource(resourceId,entity->moveResource(resourceId,amount));
        }
    }
}

void Unit::giveResource(Entity* entity)
{
    for (auto itr = mResources.begin(); itr != mResources.end(); itr++)
    {
        if (entity != nullptr)
        {
            entity->addResource(itr->first,moveResource(itr->first));
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
