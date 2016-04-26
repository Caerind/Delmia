#ifndef NDISTRIBUTIONS_HPP
#define NDISTRIBUTIONS_HPP

#include "Distribution.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Vector.hpp"

namespace Distributions
{
	Distribution<int> uniform(int min, int max);
	Distribution<unsigned int> uniform(unsigned int min, unsigned int max);
	Distribution<float> uniform(float min, float max);
	Distribution<sf::Time> uniform(sf::Time min, sf::Time max);
	Distribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize);
	Distribution<sf::Vector2f> circle(sf::Vector2f center, float radius);
	Distribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation);
}

#endif // NDISTRIBUTIONS_HPP
