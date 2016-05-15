#include "Citizen.hpp"

Citizen::Citizen()
{
    mType = Units::Citizen;
}

Citizen::Citizen(Player* player, sf::Vector2f const& pos) : Unit(player, pos)
{
    mType = Units::Citizen;
}

void Citizen::gatherResource(Resource* resource)
{
    mBuilding = nullptr;
    mResource = resource;
    if (mResource != nullptr)
    {
        positionOrder(mResource->getEntryPoint());
    }
}

void Citizen::build(Building* building)
{
    mResource = nullptr;
    mBuilding = building;
    if (mBuilding != nullptr)
    {
        positionOrder(mBuilding->getEntryPoint());
    }
}
