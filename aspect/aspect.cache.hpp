#ifndef __ASPECT_CACHE_HPP__
#define __ASPECT_CACHE_HPP__

#include <thread>
#include <mutex>

#include "aspect.hpp"

namespace pg
{
	namespace aspect
	{
		/*
			Reactors
		*/

		template <typename T, typename F>
		auto& reactors()
		{
			static std::vector<F> reactors;
			static std::mutex mutex;

			std::lock_guard<std::mutex> guard(mutex);
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
			static std::mutex mutex;
			auto function {make_function(f)};
			auto& list = reactors<T, decltype(function)>();

			{
				std::lock_guard<std::mutex> guard(mutex);
				list.push_back(function);
			}
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
		auto& invalidators_stack()
		{
			static std::map<std::thread::id, std::stack<std::function<void()>>> stack;
			static std::mutex mutex;

			std::lock_guard<std::mutex> guard(mutex);
			return stack[std::this_thread::get_id()];
		}

		template <typename T, typename... ARGS>
		auto& invalidators(ARGS... args)
		{
			static std::map<std::tuple<ARGS...>, std::vector<std::function<void()>>> invalidators;
			static std::mutex mutex;

			std::lock_guard<std::mutex> guard(mutex);
			return invalidators[std::make_tuple(args...)];
		}

		template <typename T, typename... ARGS, typename F>
		void add_invalidator(F f, ARGS... args)
		{
			static std::mutex mutex;
			auto function = make_function(f);
			auto& list = invalidators<T>(args...);

			{
				std::lock_guard<std::mutex> guard(mutex);
				list.push_back(function);
			}
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
			static std::mutex mutex;

			struct Wrapper
			{
				std::mutex& mutex;
				const std::tuple<ARGS...> key;

				Wrapper(std::mutex& m, const std::tuple<ARGS...>& k)
					: mutex(m), key(k)
				{
				}

				Wrapper& operator=(bool v)
				{
					bool old_value = false;

					{
						std::lock_guard<std::mutex> guard(mutex);
						auto& value = validity[key];
						old_value = value;
						value = v;
					}

					if(old_value)
						apply(&invalidate<T, ARGS...>, key);

					return *this;
				}

				operator bool()
				{
					std::lock_guard<std::mutex> guard(mutex);
					return validity[key];
				}
			};

			return Wrapper(mutex, key);
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
			static std::mutex mutex;

			struct Wrapper
			{
				std::mutex& mutex;
				const std::tuple<ARGS...> key;

				Wrapper(std::mutex& m, const std::tuple<ARGS...>& k)
					: mutex(m), key(k)
				{
				}

				Wrapper& operator=(const typename INAME::function::result_type& v)
				{
					{
						std::lock_guard<std::mutex> guard(mutex);
						cache[key] = v;
					}

					validity_map_key<INAME>(key) = true;
					apply(&react<INAME, ARGS...>, key);

					return *this;
				}

				operator typename INAME::function::result_type()
				{
					std::lock_guard<std::mutex> guard(mutex);
					return cache[key];
				}
			};

			return Wrapper(mutex, key);
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
			auto& invalidator_stack = invalidators_stack();

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
