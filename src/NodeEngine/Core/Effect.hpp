#ifndef NEFFECT_HPP
#define NEFFECT_HPP

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class NEffect
{
    public:
        virtual ~NEffect();

        static bool isSupported();

        virtual void apply(sf::RenderTexture const& input, sf::RenderTarget& output) = 0;

    protected:
        static void applyShader(sf::Shader const& shader, sf::RenderTarget& output);
};

#endif // NEFFECT_HPP
