#ifndef AH_AUDIOMANAGER_HPP
#define AH_AUDIOMANAGER_HPP

#include <map>
#include <memory>

#include <SFML/Audio.hpp>

namespace ah
{

class AudioSource;
class Music;
class Sound;
class AudioManager
{
    public:
        AudioManager();
        ~AudioManager();

        void registerMusicFile(std::string const& id, std::string const& filename);

        std::shared_ptr<Music> playMusic(std::string const& id, bool loop = false, sf::Vector2f const& position = sf::Vector2f());
        std::shared_ptr<Sound> playSound(std::string const& id, bool loop = false, sf::Vector2f const& position = sf::Vector2f());

        void play();
        void pause();
        void stop();

        void update();

        void setGlobalVolume(float volume);
        void setMusicVolume(float volume);
        void setSoundVolume(float volume);
        float getGlobalVolume() const;
        float getMusicVolume() const;
        float getSoundVolume() const;

        bool load(std::string const& filename);
        void save(std::string const& filename);

        sf::SoundSource::Status getStatus() const;

    protected:
        std::map<std::string,std::string> mMusicFilenames;
        std::vector<std::shared_ptr<AudioSource>> mAudioSources;
        sf::SoundSource::Status mStatus;

        float mGlobalVolume;
        float mMusicVolume;
        float mSoundVolume;
};

class AudioSource
{
    public:
        AudioSource();
        virtual ~AudioSource();

        typedef std::shared_ptr<AudioSource> Ptr;

        virtual void play();
        virtual void pause();
        virtual void stop();
        virtual void update();
        virtual sf::SoundSource::Status getStatus();

        enum Type
        {
            Music,
            Sound,
        };

        virtual void setVolume(float volume);
        virtual void setPosition(sf::Vector2f const& position);
        virtual void setLoop(bool loop);
        virtual float getVolume();
        virtual sf::Vector2f getPosition();
        virtual bool getLoop();

        Type getType() const;

    protected:
        Type mType;
};

class Music : public AudioSource
{
    public:
        Music(std::string const& filename, bool loop, sf::Vector2f const& position);

        void play();
        void pause();
        void stop();
        sf::SoundSource::Status getStatus();

        void setVolume(float volume);
        void setPosition(sf::Vector2f const& position);
        void setLoop(bool loop);
        float getVolume();
        sf::Vector2f getPosition();
        bool getLoop();

        sf::Music& getMusic();

    private:
        sf::Music mMusic;
};

class Sound : public AudioSource
{
    public:
        Sound(sf::SoundBuffer& soundBuffer, bool loop, sf::Vector2f const& position);

        void play();
        void pause();
        void stop();
        sf::SoundSource::Status getStatus();

        void setVolume(float volume);
        void setPosition(sf::Vector2f const& position);
        void setLoop(bool loop);
        float getVolume();
        sf::Vector2f getPosition();
        bool getLoop();

        sf::Sound& getSound();

    private:
        sf::Sound mSound;
};

} // namespace ah

#endif // AH_AUDIOMANAGER_HPP
