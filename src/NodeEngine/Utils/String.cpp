#include "String.hpp"

namespace NString
{

void toLower(std::string& str)
{
    std::transform(str.begin(), str.end(),str.begin(), tolower);
}

void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(),str.begin(), toupper);
}

std::string split(std::string& base, char separator)
{
    std::size_t found = base.find(separator);
    if (found != std::string::npos)
    {
        std::string t = base.substr(0, found);
        base = base.substr(found + 1);
        return t;
    }
    return "";
}

std::string split(std::string& base, std::string const& separator)
{
    std::size_t found = base.find(separator);
    if (found != std::string::npos)
    {
        std::string t = base.substr(0, found);
        base = base.substr(found + 1);
        return t;
    }
    return "";
}

std::vector<std::string> splitVector(std::string str, std::string const& separator)
{
    std::vector<std::string> v;
    std::size_t found;
    do
    {
        found = str.find(separator);
        if (found != std::string::npos)
        {
            v.push_back(str.substr(0,found));
            str = str.substr(found + 1);
        }
    } while (found != std::string::npos);
    v.push_back(str);
    return v;
}

bool contains(std::string const& str, char c)
{
    return str.find(c) != std::string::npos;
}

bool contains(std::string const& str, std::string const& c)
{
    return str.find(c) != std::string::npos;
}

std::string limitSize(std::string str, std::size_t size)
{
    if (str.size() <= size)
    {
        return str;
    }
    else
    {
        return str.substr(0,size);
    }
}

std::string toString(sf::FloatRect const& rect)
{
    return std::to_string(rect.left) + "," + std::to_string(rect.top) + "," + std::to_string(rect.width) + "," + std::to_string(rect.height);
}

std::string toString(sf::IntRect const& rect)
{
    return std::to_string(rect.left) + "," + std::to_string(rect.top) + "," + std::to_string(rect.width) + "," + std::to_string(rect.height);
}

std::string toString(sf::Color const& color)
{
    return std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a);
}

std::string toString(NVector const& v)
{
    return std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
}

sf::FloatRect toFloatRect(std::string str)
{
    sf::FloatRect r;
    r.left = std::stof(split(str,","));
    r.top = std::stof(split(str,","));
    r.width = std::stof(split(str,","));
    r.height = std::stof(str);
    return r;
}

sf::IntRect toIntRect(std::string str)
{
    sf::IntRect r;
    r.left = std::stoi(split(str,","));
    r.top = std::stoi(split(str,","));
    r.width = std::stoi(split(str,","));
    r.height = std::stoi(str);
    return r;
}

sf::Color toColor(std::string str)
{
    sf::Color c;
    c.r = std::stof(split(str,","));
    c.g = std::stof(split(str,","));
    c.b = std::stof(split(str,","));
    c.a = std::stof(str);
    return c;
}

NVector toVector(std::string str)
{
    NVector v;
    v.x = std::stof(split(str,","));
    v.y = std::stof(split(str,","));
    v.z = std::stof(str);
    return v;
}

} // namespace NString
