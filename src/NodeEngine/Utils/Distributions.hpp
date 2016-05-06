#ifndef NDISTRIBUTIONS_HPP
#define NDISTRIBUTIONS_HPP

#include <cassert>
#include <functional>
#include <type_traits>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Math.hpp"
#include "Vector.hpp"

#define ENABLE_IF(...)  , typename std::enable_if<__VA_ARGS__>::type* = nullptr

template <typename T>
class NDistribution;

namespace detail
{
    template <typename T>
	struct Constant
	{
		explicit Constant(T value) : value(value)
		{
		}

		T operator() () const
		{
			return value;
		}

		T value;
	};

	template <typename Fn, typename T>
	struct IsCompatibleFunction
	{
		static const bool value = !std::is_convertible<Fn, T>::value;
	};

	template <typename U, typename T>
	struct IsCompatibleFunction<Distribution<U>, T>
	{
		static const bool value = std::is_convertible<U, T>::value;
	};
}

/// @brief Class holding a rule to create values with predefined properties
/// @details Contains a callback that returns values on demand. These can be constant (always the same value), according to a
///  random distribution, or be read from a value elsewhere in your code. Generally, the callback can be any function, member
///  function or functor returning a value of type T and taking no arguments.
/// @n@n thor::Distribution can be linked to random distributions of the standard library:
/// @code
/// std::mt19937 engine;
/// std::uniform_int_distribution<int> distr(0, 7);
/// auto randomizer = std::bind(distr, engine);
///
/// thor::Distribution<int> thorDistr(randomizer);
/// @endcode

template <typename T>
class NDistribution
{
	private:
		typedef std::function<T()> FactoryFn;

	public:
		template <typename U>
        NDistribution(U constant
            ENABLE_IF(std::is_convertible<U, T>::value))
		: mFactory(detail::Constant<T>(constant))
		{
		}

		template <typename Fn>
        NDistribution(Fn function
            ENABLE_IF(detail::IsCompatibleFunction<Fn, T>::value))
		: mFactory(function)
		{
		}

		T operator() () const
		{
			return mFactory();
		}

	private:
		FactoryFn					mFactory;
};

namespace NDistributions
{
	NDistribution<int> uniform(int min, int max);
	NDistribution<unsigned int> uniform(unsigned int min, unsigned int max);
	NDistribution<float> uniform(float min, float max);
	NDistribution<sf::Time> uniform(sf::Time min, sf::Time max);
	NDistribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize);
	NDistribution<sf::Vector2f> circle(sf::Vector2f center, float radius);
	NDistribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation);
}

namespace NDistributions
{

namespace
{
    template <typename T>
    NDistribution<T> uniformT(T min, T max)
    {
        assert(min <= max);

        return [=] () -> T
        {
            return NMath::random(min, max);
        };
    }
}

NDistribution<int> uniform(int min, int max)
{
    return uniformT(min, max);
}

NDistribution<unsigned int> uniform(unsigned int min, unsigned int max)
{
    return uniformT(min, max);
}

NDistribution<float> uniform(float min, float max)
{
    return uniformT(min, max);
}

NDistribution<sf::Time> uniform(sf::Time min, sf::Time max)
{
    assert(min <= max);

    const float floatMin = min.asSeconds();
    const float floatMax = max.asSeconds();

    return [=] () -> sf::Time
    {
        return sf::seconds(NMath::random(floatMin, floatMax));
    };
}

NDistribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize)
{
    assert(halfSize.x >= 0.f && halfSize.y >= 0.f);
    return [=] () -> sf::Vector2f
    {
        return {NMath::randomDev(center.x, halfSize.x), NMath::randomDev(center.y, halfSize.y)};
    };
}

NDistribution<sf::Vector2f> circle(sf::Vector2f center, float radius)
{
    assert(radius >= 0.f);
    return [=] () -> sf::Vector2f
    {
        sf::Vector2f n;
        setLength(n, radius * std::sqrt(NMath::random(0.f,1.f)));
        setPolarAngle(n, NMath::random(0.f,360.f));
        return center + n;
    };
}

NDistribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation)
{
    return [=] () -> sf::Vector2f
    {
        return direction + rotatedVector(direction, NMath::randomDev(0.f,maxRotation));
    };
}

} // namespace NDistributions

#endif // NDISTRIBUTIONS_HPP
