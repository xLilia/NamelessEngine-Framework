#pragma once
#include "NamelessCore.h"
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
			//std::string ClassName() const override;

			sf::SoundBuffer SoundFile;
			sf::Sound Sound;
		};
	}
}

