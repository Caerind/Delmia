#ifndef NSTRING_HPP_INCLUDED
#define NSTRING_HPP_INCLUDED

#include <iostream>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Vector.hpp"

namespace NString
{


#ifdef N_DESKTOP_PLATFORM

#ifdef __GNUG__ // GCC

#include <cxxabi.h>
#include <cstdlib>

static std::string readable_name( const char* mangled_name )
{
    int status ;
    char* temp = __cxxabiv1::__cxa_demangle( mangled_name, nullptr, nullptr, &status ) ;
    if(temp)
    {
        std::string result(temp) ;
        std::free(temp) ;
        return result ;
    }
    else return mangled_name ;
}

#else // not GCC

std::string readable_name( const char* mangled_name ) { return mangled_name ; }

#endif // __GNUG__

// Code found here : http://www.cplusplus.com/forum/beginner/100627/
// Thanks to http://www.cplusplus.com/user/JLBorges/ for this

template < typename T > std::string type()
{ return readable_name( typeid(T).name() ) ; }

template < typename T > std::string Type( const T& obj )
{ return readable_name( typeid(obj).name() ) ; }

#endif // N_DESKTOP_PLATFORM

void toLower(std::string& str);
void toUpper(std::string& str);

std::string split(std::string& base, char separator);
std::string split(std::string& base, std::string const& separator);
std::vector<std::string> splitVector(std::string str, std::string const& separator);

bool contains(std::string const& str, char c);
bool contains(std::string const& str, std::string const& c);

std::string limitSize(std::string str, std::size_t size);

std::string toString(sf::FloatRect const& rect);
std::string toString(sf::IntRect const& rect);
std::string toString(sf::Color const& color);
std::string toString(NVector const& v);
sf::FloatRect toFloatRect(std::string str);
sf::IntRect toIntRect(std::string str);
sf::Color toColor(std::string str);
NVector toVector(std::string str);

} // namespace NString

#endif // NSTRING_HPP_INCLUDED
