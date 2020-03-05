#pragma once

#include <memory>

#include <soloud/soloud.h>

namespace obe::Audio
{
    class AudioManager;

    /**
     * \brief Enum that defines the current status of a sound
     */
    enum class SoundStatus
    {
        // The sound is currently playing
        Playing,
        // The sound is paused
        Paused,
        // The sound is stopped
        Stopped
    };

    /**
     * \brief Class to handle sounds / musics
     *        The music can be either streamed from disk or read from ram
     */
    class Sound
    {
    private:
        SoLoud::Soloud& m_manager;
        std::shared_ptr<SoLoud::AudioSource> m_source;
        SoLoud::handle m_handle;
        unsigned int m_baseSamplerate;
        float m_pitch = 1.f;
        bool m_looping = false;
        float m_volume = 1.f;
        void applyChanges();

    public:
        Sound(SoLoud::Soloud& manager, std::shared_ptr<SoLoud::AudioSource> source);
        double getDuration() const;
        void play();
        void pause();
        void stop();

        void setPitch(float pitch);
        float getPitch() const;

        SoundStatus getStatus() const;

        double getOffset() const;
        void setOffset(double offset);

        float getVolume() const;
        void setVolume(float volume);

        void setLooping(bool looping);
        bool getLooping() const;
    };
}