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
			register(IMutate<T>, (const creature<T>& c)
			{
				return creature<T>(c);
			});

			register(IGenerate<T>, ()
			{
				return creature<T>(T());
			});

			register(IGenerateWorld<T>, ()
			{
				world<T> result;
				auto ws = resolve(IGetInitialWorldSize<T>)();
				for(int i = 0 ; i < ws ; i++)
					result.emplace_back(resolve(IGenerate<T>)());
				return result;
			});

			register(IReproduce<T>, (world<T>& w)
			{
				auto reproduction_rate = resolve(IGetReproductionRate<T>)();
				auto random = resolve(IRandom<T>);
				auto select_best = resolve(ISelectBest<T>);
				auto merge = resolve(IMerge<T>);

				for(int i = 0 ; i < w.size() * reproduction_rate; i++)
				{
					auto parent_1(random(w, select_best));
					auto parent_2(random(w, select_best));
					w.emplace_back(merge(*parent_1, *parent_2));
				}
			});

			register(IEvolve<T>, (world<T>& w)
			{
				auto mutate_rate = resolve(IGetMutateRate<T>)();
				auto random = resolve(IRandom<T>);
				auto select_random = resolve(ISelectRandom<T>);
				auto mutate = resolve(IMutate<T>);

				for(int i = 0 ; i < w.size() * mutate_rate; i++)
				{
					auto creature(random(w, select_random));
					*creature = mutate(*creature);
				}
			});

			register(IKill<T>, (world<T>& w)
			{
				auto kill_rate = resolve(IGetKillRate<T>)();
				auto random = resolve(IRandom<T>);
				auto select_worst = resolve(ISelectWorst<T>);

				for(int i = 0 ; i < w.size() * kill_rate; i++)
				{
					auto creature(random(w, select_worst));
					w.erase(creature);
				}
			});

			register(IAge<T>, (world<T>& w)
			{
				for(auto &c : w)
					c.age++;
			});

			register(ISelectBest<T>, (const creature<T>& c)
			{
				return c.fitness;
			});

			register(ISelectWorst<T>, (const creature<T>& c)
			{
				return 1 - c.fitness;
			});

			register(ISelectRandom<T>, (const creature<T>& c)
			{
				return 1;
			});

			register(IRandom<T>, (world<T>& w, const typename IEvaluate<T>::function& compute)
			{
				double total = 0;
				for(auto c : w)
					total += compute(c);

				double target = resolve(IRand<T>)(0, total);
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

			register(ISelect<T>, (world<T>& w, const typename IEvaluate<T>::function& compute)
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

			register(ILifeCycle<T>, ()
			{
				if(!resolve(IEvaluate<T>))
					throw std::logic_error("IEvaluate should be overloaded");
				if(!resolve(IMerge<T>))
					throw std::logic_error("IMerge should be overloaded");
				if(!resolve(IEndOfTheWorld<T>))
					throw std::logic_error("IEndOfTheWorld should be overloaded");

				auto world(resolve(IGenerateWorld<T>)());

				while(!resolve(IEndOfTheWorld<T>)(world))
				{
					resolve(IEvolve<T>)(world);
					resolve(IReproduce<T>)(world);
					resolve(IKill<T>)(world);
					resolve(IAge<T>)(world);
				}

				return world;
			});

			register(IRand<T>, (double min, double max)
			{
				static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
				static std::default_random_engine generator(seed);
				std::uniform_real_distribution<double> distribution(min,max);
				return distribution(generator);
			});

			register(IGetInitialWorldSize<T>, () { return 100; });
			register(IGetKillRate<T>, () { return .20; });
			register(IGetReproductionRate<T>, () { return .18; });
			register(IGetMutateRate<T>, () { return .10; });
		};
	}
};

#endif
