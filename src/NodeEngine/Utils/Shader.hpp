#ifndef LP_SHADER_HPP
#define LP_SHADER_HPP

#include <SFML/Graphics/Shader.hpp>

#include <map>
#include <string>

namespace lp
{

class Shader : public sf::Shader
{
    public:
        Shader();

        void setUniform(std::string const& name, float x);
        float getUniformFloat(std::string const& name);

        // TODO : Add Shader Uniform Types : Waiting for the SFML new version documentation

    protected:
        std::map<std::string,float> mFloats;
};

} // namespace lp

#endif // LP_SHADER_HPP
