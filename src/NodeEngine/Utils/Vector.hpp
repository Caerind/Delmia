#ifndef NVECTOR_HPP
#define NVECTOR_HPP

#include <cassert>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "Math.hpp"

template <typename T>
bool isZero(sf::Vector2<T> const& vector);

template <typename T>
bool isNormalized(sf::Vector2<T> const& vector);

template <typename T>
void normalize(sf::Vector2<T>& vector);

template <typename T>
sf::Vector2<T> normalizedVector(sf::Vector2<T> const& vector);

template <typename T>
T getLength(sf::Vector2<T> const& vector);

template <typename T>
T getSquaredLength(sf::Vector2<T> const& vector);

template <typename T>
void setLength(sf::Vector2<T>& vector, T newLength);

template <typename T>
T getPolarAngle(sf::Vector2<T> const& vector);

template <typename T>
void setPolarAngle(sf::Vector2<T>& vector, T newAngle);

template <typename T>
void rotate(sf::Vector2<T>& vector, T angle);

template <typename T>
sf::Vector2<T> rotatedVector(sf::Vector2<T> const& vector, T angle);

template <typename T>
sf::Vector2<T> perpendicularVector(sf::Vector2<T> const& vector);

template <typename T>
T signedAngle(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs);

template <typename T>
T dotProduct(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs);

template <typename T>
T crossProduct(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs);

// ------------------------------------

template <typename T>
bool isZero(sf::Vector2<T> const& vector)
{
    return (vector.x == 0.f && vector.y == 0.f);
}

template <typename T>
bool isNormalized(sf::Vector2<T> const& vector)
{
    return (getSquaredLength(vector) == 1.f);
}

template <typename T>
void normalize(sf::Vector2<T>& vector)
{
    setLength(vector,1.f);
}

template <typename T>
sf::Vector2<T> normalizedVector(sf::Vector2<T> const& vector)
{
    assert(!isZero(vector));
	return vector / getLength(vector);
}

template <typename T>
T getLength(sf::Vector2<T> const& vector)
{
    return std::sqrt(getSquaredLength(vector));
}

template <typename T>
T getSquaredLength(sf::Vector2<T> const& vector)
{
    return dotProduct(vector, vector);
}

template <typename T>
void setLength(sf::Vector2<T>& vector, T newLength)
{
    assert(!isZero(vector));
    vector *= newLength / getLength(vector);
}

template <typename T>
T getPolarAngle(sf::Vector2<T> const& vector)
{
    assert(!isZero(vector));
    return NMath::atan2(vector.y,vector.x);
}

template <typename T>
void setPolarAngle(sf::Vector2<T>& vector, T newAngle)
{
    // No assert here, because turning a zero vector is well-defined (yields always zero vector)
    T vecLength = getLength(vector);
    vector.x = vecLength * NMath::cos(newAngle);
    vector.y = vecLength * NMath::sin(newAngle);
}

template <typename T>
void rotate(sf::Vector2<T>& vector, T angle)
{
    // No assert here, because turning a zero vector is well-defined (yields always zero vector)
    T cos = NMath::cos(angle);
	T sin = NMath::sin(angle);
    // Don't manipulate x and y separately, otherwise they're overwritten too early
	vector = sf::Vector2<T>(cos * vector.x - sin * vector.y, sin * vector.x + cos * vector.y);
}

template <typename T>
sf::Vector2<T> rotatedVector(sf::Vector2<T> const& vector, T angle)
{
    sf::Vector2<T> copy = vector;
	rotate(copy, angle);
	return copy;
}

template <typename T>
sf::Vector2<T> perpendicularVector(sf::Vector2<T> const& vector)
{
	return sf::Vector2<T>(-vector.y, vector.x);
}

template <typename T>
T signedAngle(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs)
{
	assert(!isZero(lhs) && !isZero(rhs));
	return NMath::atan2(crossProduct(lhs, rhs), dotProduct(lhs, rhs));
}

template <typename T>
T dotProduct(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T>
T crossProduct(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

#endif // NVECTOR_HPP
