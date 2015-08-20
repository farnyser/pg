#ifndef __PG_GA__
#define __PG_GA__

#include <random>
#include <functional>
#include <cstdlib>
#include <chrono>

namespace pg
{
	template<class T> 
	class ga 
	{
		public:			
			struct creature
			{
				T value;
				int age;
				double fitness;

				creature(const ga<T>&, const T& value);
			};

			using GA = ga<T>;
			using world = std::vector<creature>;
			using underlying = T;
			using evaluate_fct = std::function<double(const GA&, const creature&)>;
			using mutate_fct = std::function<creature(const GA&, const creature&)>;
			using merge_fct = std::function<creature(const GA&, const creature&, const creature&)>;
			using generate_fct = std::function<creature(const GA&)>;
			using generate_world_fct = std::function<world(const GA&, int)>;
			using end_of_the_world_fct = std::function<bool(const GA&, world&)>;
			using evolve_fct = std::function<void(const GA&, world&)>;
			using reproduce_fct = std::function<void(const GA&, world&)>;
			using kill_fct = std::function<void(const GA&, world&)>;
			using age_fct = std::function<void(const GA&, world&)>;
			using select_fct = std::function<typename world::iterator(const GA&, world&, const evaluate_fct&)>;
			using lifecycle_fct = std::function<world(const GA&)>;
			using rand_fct = std::function<double(const GA&, double, double)>;

			evaluate_fct evaluate;
			evaluate_fct select_best;
			evaluate_fct select_worst;
			evaluate_fct select_random;
			select_fct random;
			select_fct select;
			merge_fct merge;
			mutate_fct mutate;
			generate_fct generate;
			generate_world_fct generate_world;
			end_of_the_world_fct end_of_the_world;
			reproduce_fct reproduce;
			evolve_fct evolve;
			kill_fct kill;
			age_fct age;
			lifecycle_fct lifecycle;
			rand_fct rand;

			int world_size;
			double reproduction_rate;
			double kill_rate;
			double mutate_rate;

		public:
			ga();
	};
}

#include "ga.default.hpp"

template<class T>
pg::ga<T>::creature::creature(const GA& ga, const T& value)
 : age(0), value(value)
{
	fitness = ga.evaluate(ga, *this);
}

template<class T>
pg::ga<T>::ga() 
{
	pg::ga_default<T> gadef;

	evaluate = gadef.evaluate;
	select_best = gadef.select_best;
	select_worst = gadef.select_worst;
	select_random = gadef.select_random;
	random = gadef.random;
	select = gadef.select;
	merge = gadef.merge;
	mutate = gadef.mutate;
	generate = gadef.generate;
	generate_world = gadef.generate_world;
	end_of_the_world = gadef.end_of_the_world;
	reproduce = gadef.reproduce;
	evolve = gadef.evolve;
	kill = gadef.kill;
	age = gadef.age;
	lifecycle = gadef.lifecycle;
	rand = gadef.rand;

	world_size = gadef.world_size;
	reproduction_rate = gadef.reproduction_rate;
	kill_rate = gadef.kill_rate;
	mutate_rate = gadef.mutate_rate;
}

#endif