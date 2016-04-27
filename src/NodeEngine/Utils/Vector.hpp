#ifndef NVECTOR_HPP
#define NVECTOR_HPP

#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class NVector
{
    public:
        float x;
        float y;
        float z;

        static NVector UpVector(); // (0,0,1)
        static NVector RightVector(); // (0,1,0)
        static NVector ForwardVector(); // (1,0,0)
        static NVector ZeroVector(); // (0,0,0)

        NVector(); // (0,0,0)
        NVector(float value); // (value,value,value)
        NVector(float x, float y, float z = 0.f); // (x,y,z)

        //NVector(b2Vec2 const& vec, float z = 0.f);
        NVector(sf::Vector2f const& vec, float z = 0.f);
        NVector(sf::Vector2i const& vec, float z = 0.f);
        NVector(sf::Vector3f const& vec);

        // Functions
        bool isZero() const;

        bool isNormalized() const;
        void normalize();
        NVector unitVector() const;

        float size() const;
        float sizeSquared() const;

        float size2D() const;
        float sizeSquared2D() const;


        // Polar2D
        float getLength() const;
        float getAngle() const;

        void setLength(float length);
        void setAngle(float angle);

        void rotate(float angle);


        // Conversions
        static sf::Vector2f NToSFML2F(NVector const& v);
        static sf::Vector2i NToSFML2I(NVector const& v);
        static sf::Vector3f NToSFML3F(NVector const& v);
        static NVector SFML2FToN(sf::Vector2f const& v, float z = 0.f);
        static NVector SFML2IToN(sf::Vector2i const& v, float z = 0.f);
        static NVector SFML3FToN(sf::Vector3f const& v);
};

// Operators
NVector operator+(NVector const& left, NVector const& right);
NVector& operator+=(NVector& left, NVector const& right);
NVector operator-(NVector const& left, NVector const& right);
NVector& operator-=(NVector& left, NVector const& right);
NVector operator*(float left, NVector const& right);
NVector& operator*=(NVector& left, float right);

#endif // NVECTOR_HPP
