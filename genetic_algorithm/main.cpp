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

	register(IEndOfTheWorld<std::string>, (world<std::string>& w)
	{
		auto best = resolve(ISelect<std::string>)(w, resolve(ISelectBest<std::string>));
		auto worst = resolve(ISelect<std::string>)(w, resolve(ISelectWorst<std::string>));

		std::cout << "world "
			<< "size=" << w.size() << "; "
			<< "best=" << best->fitness << "; "
			<< "worst=" << worst->fitness << "; "
			<< std::endl;
		;

		return w.size() > 200;
	});

	register(IEvaluate<std::string>, (const creature<std::string>& c)
	{
			return resolve(IRand<std::string>)(0,1);
	});

	register(IMerge<std::string>, (const creature<std::string>& c1, const creature<std::string>& c2)
	{
		return c1.fitness > c2.fitness ? c1 : c2;
	});

	resolve(ILifeCycle<std::string>)();

	return EXIT_SUCCESS;
}
