#include "NL_Time.h"

_NL::Engine::Time::Time()
{
	Clock.restart();
	frameClk.restart();
}

_NL::Engine::Time::~Time()
{
}

void _NL::Engine::Time::Tick()
{
	DeltaTime = frameClk.restart();
}
