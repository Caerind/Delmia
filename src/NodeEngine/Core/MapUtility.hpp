#ifndef NMAPUTILITY_HPP
#define NMAPUTILITY_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>

namespace NMapUtility
{
	namespace Orthogonal
	{
		std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords, bool diag = false);
		sf::Vector2i worldToCoords(sf::Vector2f const& pos);
		sf::Vector2f coordsToWorld(sf::Vector2i const& coords); // center of the tile
	}

	namespace Isometric
	{
		std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords, bool diag = false);
		sf::Vector2i worldToCoords(sf::Vector2f const& pos);
		sf::Vector2f coordsToWorld(sf::Vector2i const& coords); // center of the tile
	}

	namespace Hexagonal
	{
		std::vector<sf::Vector2i> getNeighboors(sf::Vector2i const& coords);
		sf::Vector2i worldToCoords(sf::Vector2f const& pos);
		sf::Vector2f coordsToWorld(sf::Vector2i const& coords); // center of the tile
	}

} // namespace NMapUtility

#endif // NMAPUTILITY_HPP
