#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <cstdlib>

#include "ga.hpp"

using namespace pg::ga;

int main(int argc, char **argv)
{
	setup<std::string>();

	aspect_register(IEndOfTheWorld<std::string>, (world<std::string>& w)
	{
		using pg::aspect::call;
		using pg::aspect::ioc;

		auto best = call<ISelect<std::string>>(w, ioc<ISelectBest<std::string>>());
		auto worst = call<ISelect<std::string>>(w, ioc<ISelectWorst<std::string>>());

		std::cout << "world "
			<< "size=" << w.size() << "; "
			<< "best=" << best->fitness << "; "
			<< "worst=" << worst->fitness << "; "
			<< std::endl;
		;

		return w.size() > 200;
	});

	aspect_register(IEvaluate<std::string>, (const creature<std::string>& c)
	{
			return pg::aspect::call<IRand<std::string>>(0,1);
	});

	aspect_register(IMerge<std::string>, (const creature<std::string>& c1, const creature<std::string>& c2)
	{
		return c1.fitness > c2.fitness ? c1 : c2;
	});

	// pg::aspect::before([](const std::string& fct) { std::cout << fct << std::endl; });
	// pg::aspect::after([](const std::string& fct) { std::cout << "~" << fct << std::endl; });

	pg::aspect::call<ILifeCycle<std::string>>();

	return EXIT_SUCCESS;
}
