#include "AudioSource.h"

_NL::Engine::AudioSource::AudioSource()
{

}

void _NL::Engine::AudioSource::LoadAudioFile(const char * Filepath)
{
	SoundFile.loadFromFile(Filepath);
	Sound.setBuffer(SoundFile);
}

//std::string _NL::Engine::AudioSource::getTypeName() const
//{
//	return "_NL::Component::AudioSource";
//}

_NL::Engine::AudioSource::~AudioSource()
{
}
