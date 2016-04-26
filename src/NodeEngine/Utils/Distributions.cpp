#include "Distributions.hpp"
#include "Math.hpp"

#include <cassert>

namespace Distributions
{

namespace
{
    template <typename T>
    Distribution<T> uniformT(T min, T max)
    {
        assert(min <= max);

        return [=] () -> T
        {
            return NMath::random(min, max);
        };
    }
}

Distribution<int> uniform(int min, int max)
{
    return uniformT(min, max);
}

Distribution<unsigned int> uniform(unsigned int min, unsigned int max)
{
    return uniformT(min, max);
}

Distribution<float> uniform(float min, float max)
{
    return uniformT(min, max);
}

Distribution<sf::Time> uniform(sf::Time min, sf::Time max)
{
    assert(min <= max);

    const float floatMin = min.asSeconds();
    const float floatMax = max.asSeconds();

    return [=] () -> sf::Time
    {
        return sf::seconds(NMath::random(floatMin, floatMax));
    };
}

Distribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize)
{
    assert(halfSize.x >= 0.f && halfSize.y >= 0.f);

    return [=] () -> sf::Vector2f
    {
        return sf::Vector2f(NMath::randomDev(center.x, halfSize.x),NMath::randomDev(center.y, halfSize.y));
    };
}

Distribution<sf::Vector2f> circle(sf::Vector2f center, float radius)
{
    assert(radius >= 0.f);

    return [=] () -> sf::Vector2f
    {
        NVector n;
        n.setLength(radius * std::sqrt(NMath::random(0.f,1.f)));
        n.setAngle(NMath::random(0.f,360.f));
        return center + NVector::NToSFML2F(n);
    };
}

Distribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation)
{
    return [=] () -> sf::Vector2f
    {
        NVector n;
        n.rotate(NMath::randomDev(0.f,maxRotation));
        return direction + NVector::NToSFML2F(n);
    };
}

} // namespace Distributions
