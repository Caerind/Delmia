#ifndef BUILDINGS_HPP_INCLUDED
#define BUILDINGS_HPP_INCLUDED

#include "Barrack.hpp"
#include "Hall.hpp"
#include "Market.hpp"

namespace Buildings
{

enum Type
{
    DefaultBuilding = 0,
    Barrack,
    Hall,
    Market,


    DefaultResource = 500,
    Quarry,
    Forest,
    GoldMine,
};

} // namespace Buildings

#endif // BUILDINGS_HPP_INCLUDED
