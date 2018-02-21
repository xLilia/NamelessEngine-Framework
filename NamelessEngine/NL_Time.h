#pragma once
#include <SFML\System.hpp>
#include <iostream>
namespace _NL {
	namespace Engine {
		class Time
		{
		public:
			Time();
			~Time();
			sf::Time DeltaTime;
			sf::Clock Clock;
			void Tick();
		private:
			sf::Clock Clk;
		};
	}
}
