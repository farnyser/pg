#ifndef __PG_GA_DEFAULT__
#define __PG_GA_DEFAULT__

#include "ga.hpp"

namespace pg
{
	namespace ga
	{
		template<class T>
		void setup()
		{
			aspect_register(IMutate<T>, (const creature<T>& c)
			{
				return creature<T>(c);
			});

			aspect_register(IGenerate<T>, ()
			{
				return creature<T>(T());
			});

			aspect_register(IGenerateWorld<T>, ()
			{
				world<T> result;
				auto ws = pg::aspect::call<IGetInitialWorldSize<T>>();
				for(int i = 0 ; i < ws ; i++)
					result.emplace_back(pg::aspect::call<IGenerate<T>>());
				return result;
			});

			aspect_register(IReproduce<T>, (world<T>& w)
			{
				auto reproduction_rate = pg::aspect::call<IGetReproductionRate<T>>();
				auto select_best = pg::aspect::ioc<ISelectBest<T>>();

				for(int i = 0 ; i < w.size() * reproduction_rate; i++)
				{
					auto parent_1 = (pg::aspect::call<IRandom<T>>(w, select_best));
					auto parent_2 = (pg::aspect::call<IRandom<T>>(w, select_best));
					w.emplace_back(pg::aspect::call<IMerge<T>>(*parent_1, *parent_2));
				}
			});

			aspect_register(IEvolve<T>, (world<T>& w)
			{
				auto mutate_rate = pg::aspect::call<IGetMutateRate<T>>();
				auto select_random = pg::aspect::ioc<ISelectRandom<T>>();

				for(int i = 0 ; i < w.size() * mutate_rate; i++)
				{
					auto creature(pg::aspect::call<IRandom<T>>(w, select_random));
					*creature = pg::aspect::call<IMutate<T>>(*creature);
				}
			});

			aspect_register(IKill<T>, (world<T>& w)
			{
				using pg::aspect::call;

				auto kill_rate = call<IGetKillRate<T>>();
				auto select_worst = pg::aspect::ioc<ISelectWorst<T>>();

				for(int i = 0 ; i < w.size() * kill_rate; i++)
				{
					auto creature(call<IRandom<T>>(w, select_worst));
					w.erase(creature);
				}
			});

			aspect_register(IAge<T>, (world<T>& w)
			{
				for(auto &c : w)
					c.age++;
			});

			aspect_register(ISelectBest<T>, (const creature<T>& c)
			{
				return c.fitness;
			});

			aspect_register(ISelectWorst<T>, (const creature<T>& c)
			{
				return 1 - c.fitness;
			});

			aspect_register(ISelectRandom<T>, (const creature<T>& c)
			{
				return 1;
			});

			aspect_register(IRandom<T>, (world<T>& w, const typename IEvaluate<T>::function& compute)
			{
				double total = 0;
				for(auto c : w)
					total += compute(c);

				double target = pg::aspect::call<IRand<T>>(0, total);
				double current = 0;

				while(1)
				{
					for(auto it = w.begin() ; it != w.end() ; ++it)
					{
						current += compute(*it);
						if(current >= target)
							return it;
					}
				}
			});

			aspect_register(ISelect<T>, (world<T>& w, const typename IEvaluate<T>::function& compute)
			{
				auto select = w.begin();

				for(auto it = w.begin() ; it != w.end() ; ++it)
				{
					auto current = compute(*it);
					if(current >= compute(*select))
						select = it;
				}

				return select;
			});

			aspect_register(ILifeCycle<T>, ()
			{
				using pg::aspect::call;

				if(!pg::aspect::ioc<IEvaluate<T>>())
					throw std::logic_error("IEvaluate should be overloaded");
				if(!pg::aspect::ioc<IMerge<T>>())
					throw std::logic_error("IMerge should be overloaded");
				if(!pg::aspect::ioc<IEndOfTheWorld<T>>())
					throw std::logic_error("IEndOfTheWorld should be overloaded");

				auto world(call<IGenerateWorld<T>>());

				while(!call<IEndOfTheWorld<T>>(world))
				{
					call<IEvolve<T>>(world);
					call<IReproduce<T>>(world);
					call<IKill<T>>(world);
					call<IAge<T>>(world);
				}

				return world;
			});

			aspect_register(IRand<T>, (double min, double max)
			{
				static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
				static std::default_random_engine generator(seed);
				std::uniform_real_distribution<double> distribution(min,max);
				return distribution(generator);
			});

			aspect_register(IGetInitialWorldSize<T>, () { return 100; });
			aspect_register(IGetKillRate<T>, () { return .20; });
			aspect_register(IGetReproductionRate<T>, () { return .18; });
			aspect_register(IGetMutateRate<T>, () { return .10; });
		};
	}
};

#endif
