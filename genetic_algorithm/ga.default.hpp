#ifndef __PG_GA_DEFAULT__
#define __PG_GA_DEFAULT__

#include "ga.hpp"

namespace pg
{
	template<class T> 
	class ga_default
	{
		public:
			using GA = ga<T>;
			using W = typename ga<T>::world;
			using C = typename ga<T>::creature;

			typename GA::evaluate_fct evaluate = nullptr;
			
			typename GA::merge_fct merge = nullptr;
			
			typename GA::mutate_fct mutate = [](const GA& ga, const C& c)
			{ 
				return C(c); 
			};
			
			typename GA::generate_fct generate = [](const GA& ga)
			{ 
				return C(ga, typename GA::underlying()); 
			};
						
			typename GA::generate_world_fct generate_world = [](const GA& ga, int ws)
			{ 
				typename GA::world world;
				for(int i = 0 ; i < ws ; i++)
					world.emplace_back(ga.generate(ga));
				return world; 
			};
			
			typename GA::end_of_the_world_fct end_of_the_world;
			
			typename GA::reproduce_fct reproduce = [](const GA& ga, W& w)
			{
				for(int i = 0 ; i < w.size() * ga.reproduction_rate; i++)
				{
					auto parent_1(ga.random(ga, w, ga.select_best));
					auto parent_2(ga.random(ga, w, ga.select_best));
					w.emplace_back(ga.merge(ga, *parent_1, *parent_2));
				}
			};

			typename GA::evolve_fct evolve = [](const GA& ga, W& w) 
			{
				for(int i = 0 ; i < w.size() * ga.mutate_rate; i++)
				{
					auto creature(ga.random(ga, w, ga.select_random));
					*creature = ga.mutate(ga, *creature);
				}
			};
			
			typename GA::kill_fct kill = [](const GA& ga, W& w)
			{
				for(int i = 0 ; i < w.size() * ga.kill_rate; i++)
				{
					auto creature(ga.random(ga, w, ga.select_worst));
					w.erase(creature);
				}
			};

			typename GA::age_fct age = [](const GA& ga, W& w)
			{
				for(auto &c : w)
					c.age++;
			};
		
			typename GA::evaluate_fct select_best = [](const GA& ga, const C& c) 
			{
				return c.fitness;
			};

			typename GA::evaluate_fct select_worst = [](const GA& ga, const C& c) 
			{
				return 1 - c.fitness;
			};

			typename GA::evaluate_fct select_random = [](const GA& ga, const C& c) 
			{
				return 1;
			};

			typename GA::select_fct random = [](const GA& ga, W& w, const typename GA::evaluate_fct& compute)
			{
				double total = 0;
				for(auto c : w)
					total += compute(ga, c);
				
				double target = ga.rand(ga, 0, total);
				double current = 0;
				
				while(1)
				{
					for(auto it = w.begin() ; it != w.end() ; ++it)
					{
						current += compute(ga, *it);
						if(current >= target)
							return it;
					}
				}
			};

			typename GA::select_fct select = [](const GA& ga, W& w, const typename GA::evaluate_fct& compute)
			{
				auto select = w.begin();

				for(auto it = w.begin() ; it != w.end() ; ++it)
				{
					auto current = compute(ga, *it);
					
					if(current >= compute(ga, *select))
						select = it;
				}

				return select;
			};

			typename GA::lifecycle_fct lifecycle = [](const GA& ga)
			{
				if(!ga.evaluate)
					throw std::logic_error("pg::ga::evaluate should be overloaded");
				if(!ga.merge)
					throw std::logic_error("pg::ga::merge should be overloaded");
				if(!ga.end_of_the_world)
					throw std::logic_error("pg::ga::end_of_the_world should be overloaded");
				
				auto world(ga.generate_world(ga, ga.world_size));
				
				while(!ga.end_of_the_world(ga, world)) 
				{
					ga.evolve(ga, world);
					ga.reproduce(ga, world);
					ga.kill(ga, world);
					ga.age(ga, world);
				}
				
				return world;
			};

			typename GA::rand_fct rand = [](const GA& ga, double min, double max)
			{
				static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
				static std::default_random_engine generator(seed);
				std::uniform_real_distribution<double> distribution(min,max);
				return distribution(generator);
			};

			int world_size = 100;
			double kill_rate = 0.20;
			double reproduction_rate = 0.18;
			double mutate_rate = 0.10;
	};
};

#endif