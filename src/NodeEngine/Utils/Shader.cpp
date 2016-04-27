#include "Shader.hpp"

namespace lp
{

Shader::Shader()
{
}

void Shader::setUniform(std::string const& name, float x)
{
    sf::Shader::setParameter(name,x);
    mFloats[name] = x;
}

float Shader::getUniformFloat(std::string const& name)
{
    return mFloats[name];
}

} //namespace lp
