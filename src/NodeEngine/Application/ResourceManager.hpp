#ifndef AH_RESOURCEMANAGER_HPP
#define AH_RESOURCEMANAGER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <iostream>
#include <cassert>

namespace ah
{

class ResourceManager
{
    public:
        ResourceManager();
        ~ResourceManager();

        void loadTexture(std::string const& id, std::string const& filename);
        void loadImage(std::string const& id, std::string const& filename);
        void loadFont(std::string const& id, std::string const& filename);
        void loadSoundBuffer(std::string const& id, std::string const& filename);

        sf::Texture& getTexture(std::string const& id);
        sf::Image& getImage(std::string const& id);
        sf::Font& getFont(std::string const& id);
        sf::SoundBuffer& getSoundBuffer(std::string const& id);

        void releaseTexture(std::string const& id);
        void releaseImage(std::string const& id);
        void releaseFont(std::string const& id);
        void releaseSoundBuffer(std::string const& id);

    protected:
        std::map<std::string,sf::Texture> mTextures;
        std::map<std::string,sf::Image> mImages;
        std::map<std::string,sf::Font> mFonts;
        std::map<std::string,sf::SoundBuffer> mSoundBuffers;
};

} // namespace ah

#endif // AH_RESOURCEMANAGER_HPP
