#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <cstdlib>

#include "ga.hpp"

int main(int argc, char **argv)
{
	using evolution = pg::ga<std::string>;
	using creature = evolution::creature;
	using world = evolution::world;

	evolution e;
	e.evaluate = [](const evolution& e, const creature& c) {
		return e.rand(e, 0, 1);
	};
	e.end_of_the_world = [](const evolution& e, world& w) {
		auto size = w.size();
		auto best = e.select(e, w, e.select_best);
		auto worst = e.select(e, w, e.select_worst);
		std::cout 
			<< "size: " << size 
			<< "; best: " << best->fitness 
			<< "; worst: " << worst->fitness 
		<< std::endl;
		return size > 1000;
	};
	e.merge = [](const evolution& e, const creature& c1, const creature& c2) {
		return c1.fitness > c2.fitness ? c1 : c2;
	};
	
	auto w = e.lifecycle(e);
	
	return EXIT_SUCCESS;
}
