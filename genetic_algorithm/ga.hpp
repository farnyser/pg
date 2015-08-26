#ifndef __PG_GA__
#define __PG_GA__

#include "ioc/ioc.hpp"
#include <random>
#include <functional>
#include <cstdlib>
#include <chrono>

namespace pg
{
	namespace ga
	{
		template<typename T>
		struct creature
		{
			T value;
			int age;
			double fitness;

			creature(const T& value);
		};

		template<typename T>
		using world = std::vector<creature<T>>;

		template<typename T>
		resolvable(IEvaluate, double(const creature<T>&));

		template<typename T>
		resolvable(ISelectBest, double(const creature<T>&));

		template<typename T>
		resolvable(ISelectWorst, double(const creature<T>&));

		template<typename T>
		resolvable(ISelectRandom, double(const creature<T>&));

		template<typename T>
		resolvable(IMutate, creature<T>(const creature<T>&));

		template<typename T>
		resolvable(IMerge, creature<T>(const creature<T>&, const creature<T>&));

		template<typename T>
		resolvable(IGenerate, creature<T>());

		template<typename T>
		resolvable(IGenerateWorld, world<T>());

		template<typename T>
		resolvable(IEndOfTheWorld, bool(world<T>&));

		template<typename T>
		resolvable(IEvolve, void(world<T>&));

		template<typename T>
		resolvable(IReproduce, void(world<T>&));

		template<typename T>
		resolvable(IKill, void(world<T>&));

		template<typename T>
		resolvable(IAge, void(world<T>&));

		template<typename T>
		resolvable(ILifeCycle, world<T>());

		template<typename T>
		resolvable(ISelect, typename world<T>::iterator(world<T>&, const typename IEvaluate<T>::function&));

		template<typename T>
		resolvable(IRandom, typename world<T>::iterator(world<T>&, const typename IEvaluate<T>::function&));

		template<typename T>
		resolvable(IRand, double(double, double));

		template<typename T>
		resolvable(IGetInitialWorldSize, int());

		template<typename T>
		resolvable(IGetReproductionRate, double());

		template<typename T>
		resolvable(IGetKillRate, double());

		template<typename T>
		resolvable(IGetMutateRate, double());
	};
}

template<typename T>
pg::ga::creature<T>::creature(const T& value)
 : age(0), value(value)
{
	fitness = resolve(IEvaluate<T>)(*this);
}

#include "ga.default.hpp"

#endif
