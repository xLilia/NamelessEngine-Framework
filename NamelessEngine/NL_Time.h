#pragma once
#include <SFML\System.hpp>
#include <iostream>
namespace _NL {
	namespace Engine {
		static class Time
		{
		public:
			Time();
			~Time();
			sf::Time DeltaTime;
			void Tick();
		private:
			sf::Clock Clk;
		};
	}
}
