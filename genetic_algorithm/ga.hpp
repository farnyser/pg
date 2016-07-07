#ifndef __PG_GA__
#define __PG_GA__

#include "aspect/aspect.hpp"
#include "aspect/aspect.helpers.hpp"
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
		aspect_resolvable(IEvaluate, double(const creature<T>&));

		template<typename T>
		aspect_resolvable(ISelectBest, double(const creature<T>&));

		template<typename T>
		aspect_resolvable(ISelectWorst, double(const creature<T>&));

		template<typename T>
		aspect_resolvable(ISelectRandom, double(const creature<T>&));

		template<typename T>
		aspect_resolvable(IMutate, creature<T>(const creature<T>&));

		template<typename T>
		aspect_resolvable(IMerge, creature<T>(const creature<T>&, const creature<T>&));

		template<typename T>
		aspect_resolvable(IGenerate, creature<T>());

		template<typename T>
		aspect_resolvable(IGenerateWorld, world<T>());

		template<typename T>
		aspect_resolvable(IEndOfTheWorld, bool(world<T>&));

		template<typename T>
		aspect_resolvable(IEvolve, void(world<T>&));

		template<typename T>
		aspect_resolvable(IReproduce, void(world<T>&));

		template<typename T>
		aspect_resolvable(IKill, void(world<T>&));

		template<typename T>
		aspect_resolvable(IAge, void(world<T>&));

		template<typename T>
		aspect_resolvable(ILifeCycle, world<T>());

		template<typename T>
		aspect_resolvable(ISelect, typename world<T>::iterator(world<T>&, const typename IEvaluate<T>::function&));

		template<typename T>
		aspect_resolvable(IRandom, typename world<T>::iterator(world<T>&, const typename IEvaluate<T>::function&));

		template<typename T>
		aspect_resolvable(IRand, double(double, double));

		template<typename T>
		aspect_resolvable(IGetInitialWorldSize, int());

		template<typename T>
		aspect_resolvable(IGetReproductionRate, double());

		template<typename T>
		aspect_resolvable(IGetKillRate, double());

		template<typename T>
		aspect_resolvable(IGetMutateRate, double());
	};
}

template<typename T>
pg::ga::creature<T>::creature(const T& value)
 : age(0), value(value)
{
	fitness = pg::aspect::call<IEvaluate<T>>(*this);
}

#include "ga.default.hpp"

#endif
