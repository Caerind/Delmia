#include "Vector.hpp"

NVector NVector::UpVector()
{
    return NVector(0.f,0.f,1.f);
}

NVector NVector::RightVector()
{
    return NVector(0.f,1.f,0.f);
}

NVector NVector::ForwardVector()
{
    return NVector(1.f,0.f,0.f);
}

NVector NVector::ZeroVector()
{
    return NVector(0.f,0.f,0.f);
}

NVector::NVector()
{
    x = 0.f;
    y = 0.f;
    z = 0.f;
}

NVector::NVector(float value)
{
    x = value;
    y = value;
    z = value;
}

NVector::NVector(float xA, float yA, float zA)
{
    x = xA;
    y = yA;
    z = zA;
}

NVector::NVector(sf::Vector2f const& vec, float zA)
{
    NVector v = SFML2FToN(vec);
    x = v.x;
    y = v.y;
    z = zA;
}

NVector::NVector(sf::Vector2i const& vec, float zA)
{
    NVector v = SFML2IToN(vec);
    x = v.x;
    y = v.y;
    z = zA;
}

NVector::NVector(sf::Vector3f const& vec)
{
    NVector v = SFML3FToN(vec);
    x = v.x;
    y = v.y;
    z = v.z;
}

bool NVector::isZero() const
{
    return (x == 0.f && y == 0.f && z == 0.f);
}

bool NVector::isNormalized() const
{
    return size() == 1.f;
}

void NVector::normalize()
{
    float n = size();
    if (n != 0.f)
    {
        x /= n;
        y /= n;
        z /= n;
    }
}

NVector NVector::unitVector() const
{
    NVector t;
    t.x = x;
    t.y = y;
    t.z = z;
    t.normalize();
    return t;
}

float NVector::size() const
{
    return std::sqrt(sizeSquared());
}

float NVector::sizeSquared() const
{
    return (x*x + y*y + z*z);
}

float NVector::size2D() const
{
    return std::sqrt(sizeSquared2D());
}

float NVector::sizeSquared2D() const
{
    return (x*x + y*y);
}

float NVector::getLength() const
{
    return size2D();
}

float NVector::getAngle() const
{
    if (y != 0.f || x != 0.f)
    {
        return std::atan2(y,x);
    }
    return 0.f;
}

void NVector::setLength(float length)
{
    float l = getLength();
    if (getLength() != 0.f)
    {
        float r = length / l;
        x *= r;
        y *= r;
    }
}

void NVector::setAngle(float angle)
{
    float length = getLength();
	x = length * std::cos(angle);
	y = length * std::sin(angle);
}

void NVector::rotate(float angle)
{
    float cos = std::cos(angle);
	float sin = std::sin(angle);
	*this = NVector(cos * x - sin * y, sin * x + cos * y, z); // Don't manipulate x and y separately, otherwise they're overwritten too early
}

sf::Vector2f NVector::NToSFML2F(NVector const& v)
{
    return sf::Vector2f(v.x,v.y);
}

sf::Vector2i NVector::NToSFML2I(NVector const& v)
{
    return sf::Vector2i((int)v.x,(int)v.y);
}

sf::Vector3f NVector::NToSFML3F(NVector const& v)
{
    return sf::Vector3f(v.x,v.y,v.z);
}

NVector NVector::SFML2FToN(sf::Vector2f const& v, float z)
{
    return NVector(v.x,v.y,z);
}

NVector NVector::SFML2IToN(sf::Vector2i const& v, float z)
{
    return NVector((float)v.x,(float)v.y,z);
}

NVector NVector::SFML3FToN(sf::Vector3f const& v)
{
    return NVector(v.x,v.y,v.z);
}

// Operators
NVector operator +(NVector const& left, NVector const& right)
{
    return NVector(left.x + right.x, left.y + right.y, left.z + right.z);
}

NVector& operator +=(NVector& left, NVector const& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}

NVector operator-(NVector const& left, NVector const& right)
{
    return NVector(left.x - right.x, left.y - right.y, left.z - right.z);
}

NVector& operator-=(NVector& left, NVector const& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}

NVector operator *(float left, NVector const& right)
{
    return NVector(right.x * left, right.y * left, right.z * left);
}

NVector& operator *=(NVector& left, float right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;
    return left;
}
