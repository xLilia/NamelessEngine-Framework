#include "NL_Time.h"

_NL::Engine::Time::Time()
{
	
}

_NL::Engine::Time::~Time()
{
}

void _NL::Engine::Time::Tick()
{
	DeltaTime = Clk.restart();
}
