#ifndef __PG_CACHE__
#define __PG_CACHE__

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <functional>
#include <type_traits>

#include "cache.helpers.hpp"

namespace pg
{
namespace cache
{
	std::stack<std::function<void()>> context_invalidator;

	template<typename T = void, typename... ARGS>
	class cache
	{
	public:
		typedef std::function<T(ARGS...)> function_t;
		typedef std::tuple<ARGS...> key_t;
		
		template<typename... Args>
		cache(const std::string& n, Args... args) : 
			name(n), 
			key(tuple<ARGS...>::make(args...)), 
			key_set(tuple<ARGS...>::valid(args...))
		{
			if(sizeof...(Args) != 0 && !key_set)
				throw std::runtime_error("Invalid parameters provided");
		};

		template<typename... Args>
		bool valid(Args... args) const 
		{
			if(key_set && sizeof...(Args) == 0)
				return valididty(name, key);
			else
				return cache<T, Args...>::valididty(name, std::make_tuple(args...));
		}

		template<typename... Args>
		T operator()(Args... args) const
		{
			// todo use key if key_set
			return cache<T, Args...>::getValue(name, args...);
		};

		template<typename... Args>
		T cached(Args... args) const
		{
			if(key_set && sizeof...(Args) == 0)
				return getCachedValue(name, key);
			else
				return cache<T, Args...>::getCachedValue(name, std::make_tuple(args...));
		};

		cache& operator=(T value) 
		{
			if(!key_set) 
			{
				setValue(name, value);
			}
			else 
			{
				if(valididty(name, key))
					invalidate(name, key);
				cached(name, key) = value;
				valididty(name, key) = true;
			}

			return *this;
		};
		
		template<typename... Args>
		cache& operator=(const std::function<T(Args...)>& function) 
		{
			if(key_set && sizeof...(ARGS))
				throw std::runtime_error("Can't assign computer callback to specific value of " + name);

			cache<T, Args...>::set(name, function);
			return *this;
		};

		cache& operator=(const std::function<T(ARGS...)>& function) 
		{			
			if(key_set && sizeof...(ARGS))
				throw std::runtime_error("Can't assign computer callback to specific value of " + name);

			set(name, function);
			return *this;
		};

	private:
		const std::string name;
		const key_t key;
		const bool key_set;

		template<typename To, typename... ARGSo> 
		friend class cache;

		static std::map<std::string, std::map<key_t, std::vector<std::function<void()>>>> invalidators;
		static std::map<std::string, std::map<key_t, bool>> valididty_map;
		
		static bool& valididty(const std::string& name, const key_t& key)
		{
			return valididty_map[name][key];
		}

		static void invalidate(const std::string& name, const key_t& key) 
		{
			valididty(name, key) = false;
			for(auto& invalidator : invalidators[name][key])
				invalidator();
		};

		static void invalidate_all(const std::string& name) 
		{
			for(auto& kv : cache<T, ARGS...>::valididty_map[name]) 
				invalidate(name, kv.first);
		};

		static void set(const std::string& name, const function_t& f) 
		{
			if(get(name))
				cache<T, ARGS...>::invalidate_all(name);
			get(name) = f;
		};

		static function_t& get(const std::string& name) 
		{
			static std::map<std::string, function_t> functions;
			return functions[name];
		};

		static T& cached(const std::string& name, const key_t& key)
		{
			static std::map<std::string, std::map<key_t, T>> cache;
			return cache[name][key];
		}

		static T getValue(const std::string& name, ARGS... args)
		{
			const key_t key(args...);

			if(!context_invalidator.empty())
				invalidators[name][key].push_back(context_invalidator.top());
			
			context_invalidator.push([=](){ 
				cache<T, ARGS...>::invalidate(name, key); 
			});

			if(!valididty(name, key)) 
			{
				cached(name, key) = get(name)(args...);
				valididty(name, key) = true;
			};

			context_invalidator.pop();

			return cached(name, key);
		}

		static T getCachedValue(const std::string& name, const key_t& key)
		{
			return cached(name, key);
		}

		static void setValue(const std::string& name, const T& value)
		{
			cache<T, ARGS...>::invalidate_all(name);
			cache<T, ARGS...>::set(name, [value](ARGS... args) { return value; });
		}
	};
	
	template<typename T, typename... Args>
	std::map<std::string, std::map<std::tuple<Args...>, std::vector<std::function<void()>>>> cache<T,Args...>::invalidators;

	template<typename T, typename... Args>
	std::map<std::string, std::map<std::tuple<Args...>, bool>> cache<T,Args...>::valididty_map;
}
}

#endif