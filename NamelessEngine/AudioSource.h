#pragma once
#include "NamelessCore.hpp"
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Music.hpp>
namespace _NL {
	namespace Engine {
		class AudioSource
		{
		public:
			AudioSource();
			~AudioSource();

			void LoadAudioFile(const char* Filepath);
			//virtual char* getTypeName() const override;

			sf::SoundBuffer SoundFile;
			sf::Sound Sound;
		};
	}
}

