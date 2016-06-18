#ifndef __ASPECT_CACHE_HPP__
#define __ASPECT_CACHE_HPP__

#include "aspect.hpp"

namespace pg
{
	namespace aspect
	{
		// keep call stack for dependency detection
		// will have to change for thread safety
		std::stack<std::function<void()>> invalidator_stack;

		/*
			Reactors
		*/

		template <typename T, typename F>
		auto& reactors()
		{
			static std::vector<F> reactors;
			return reactors;
		}

		template <typename T, typename... ARGS>
		auto& reactors(ARGS... args)
		{
			return reactors<T, std::function<void(ARGS...)>>();
		}

		template <typename T, typename F>
		void add_reactor(F f)
		{
			auto function {make_function(f)};
			reactors<T, decltype(function)>().push_back(function);
		}

		template <typename INAME, typename... ARGS>
		void react(ARGS... args)
		{
			for(auto& reactor: reactors<INAME>(args...))
				reactor(args...);
		}

		/*
			invalidators
		*/

		template <typename T, typename... ARGS>
		auto& invalidators(ARGS... args)
		{
			static std::map<std::tuple<ARGS...>, std::vector<std::function<void()>>> invalidators;
			return invalidators[std::make_tuple(args...)];
		}

		template <typename T, typename... ARGS, typename F>
		void add_invalidator(F f, ARGS... args)
		{
			auto function = make_function(f);
			invalidators<T>(args...).push_back(function);
		}

		template <typename INAME, typename... ARGS>
		void invalidate(ARGS... args)
		{
			for(auto& invalidator: invalidators<INAME>(args...))
				invalidator();
		}

		/*
			Validity
		*/

		template <typename T, typename... ARGS>
		auto validity_map_key(const std::tuple<ARGS...>& key)
		{
			static std::map<std::tuple<ARGS...>, bool> validity;

			struct Wrapper
			{
				const std::tuple<ARGS...> key;

				Wrapper(const std::tuple<ARGS...>& k) : key(k)
				{
				}

				Wrapper& operator=(bool v)
				{
					auto& target = validity[key];
					if(target)
						apply(&invalidate<T, ARGS...>, key);

					target = v;
					return *this;
				}

				operator bool()
				{
					return validity[key];
				}
			};

			return Wrapper(key);
		}

		template <typename T, typename... ARGS>
		auto validity(ARGS... args)
		{
			return validity_map_key<T>(std::make_tuple(args...));
		}

		/*
			Cache
		*/

		template <typename INAME, typename... ARGS>
		auto cache_map_key(const std::tuple<ARGS...>& key)
		{
			static std::map<std::tuple<ARGS...>, typename INAME::function::result_type> cache;

			struct Wrapper
			{
				const std::tuple<ARGS...> key;

				Wrapper(const std::tuple<ARGS...>& k) : key(k)
				{
				}

				Wrapper& operator=(const typename INAME::function::result_type& v)
				{
					cache[key] = v;
					validity_map_key<INAME>(key) = true;
					apply(&react<INAME, ARGS...>, key);
					return *this;
				}

				operator typename INAME::function::result_type()
				{
					return cache[key];
				}
			};

			return Wrapper(key);
		}

		template <typename T, typename... ARGS>
		auto cached(ARGS... args)
		{
			return cache_map_key<T>(std::make_tuple(args...));
		}

		/*
			Aspect overload
		*/

		template <typename INAME, typename F, typename... ARGS>
		auto cache_overload(F function, ARGS... args)
		{
			if(!invalidator_stack.empty()) {
				auto inv = invalidator_stack.top();
				add_invalidator<INAME>(inv, args...);
			}

			invalidator_stack.push([args...]() {
				validity<INAME>(args...) = false;
			});

			auto value = cached<INAME>(args...);

			if(!validity<INAME>(args...))
				value = function(args...);

			invalidator_stack.pop();

			return value;
		}

		template <typename INAME>
		void cached()
		{
			typename INAME::function function = pg::aspect::ioc<INAME>();
			pg::aspect::ioc<INAME>() = [&, function] (auto... args) {
				return pg::aspect::cache_overload<INAME>(function, args...);
			};
		}
	}
}
#endif
