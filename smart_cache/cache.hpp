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
	class cache_t
	{
	public:
		typedef std::function<T(ARGS...)> function_t;
		typedef std::tuple<ARGS...> key_t;
		
		template<typename... Args>
		cache_t(const std::string& n, Args... args) : 
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
				return cache_t<T, Args...>::valididty(name, std::make_tuple(args...));
		}

		template<typename... Args>
		T operator()(Args... args) const
		{			
			if(key_set && sizeof...(Args) == 0) 
				return getValue(name, key);
			else
				return cache_t<T, Args...>::getValue(name, std::make_tuple(args...));
		};

		template<typename... Args>
		T cached(Args... args) const
		{
			if(key_set && sizeof...(Args) == 0)
				return getCachedValue(name, key);
			else
				return cache_t<T, Args...>::getCachedValue(name, std::make_tuple(args...));
		};

		template<typename O>
		cache_t& operator=(O value) 
		{
			if(!key_set) 
			{
				cache_t<O,ARGS...>::setValue(name, value);
			}
			else 
			{
				if(cache_t<O,ARGS...>::valididty(name, key))
					cache_t<O,ARGS...>::invalidate(name, key);
				cache_t<O,ARGS...>::cached(name, key) = value;
				cache_t<O,ARGS...>::valididty(name, key) = true;
			}

			return *this;
		}
		
		template<typename O, typename... Args>
		cache_t& operator=(const std::function<O(Args...)>& function) 
		{
			if(key_set && sizeof...(ARGS))
				throw std::runtime_error("Can't assign computer callback to specific value of " + name);

			cache_t<O, Args...>::set(name, function);
			return *this;
		};

		template<typename O>
		cache_t& operator=(const std::function<O(ARGS...)>& function) 
		{			
			if(key_set && sizeof...(ARGS))
				throw std::runtime_error("Can't assign computer callback to specific value of " + name);

			cache_t<O,ARGS...>::set(name, function);
			return *this;
		};

	private:
		const std::string name;
		const key_t key;
		const bool key_set;

		template<typename To, typename... ARGSo> 
		friend class cache_t;

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
			for(auto& kv : cache_t<T, ARGS...>::valididty_map[name]) 
				invalidate(name, kv.first);
		};

		static void set(const std::string& name, const function_t& f) 
		{
			if(get(name))
				cache_t<T, ARGS...>::invalidate_all(name);
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

		static T getValue(const std::string& name, const key_t& key)
		{
			if(!context_invalidator.empty())
				invalidators[name][key].push_back(context_invalidator.top());
			
			context_invalidator.push([=](){ 
				cache_t<T, ARGS...>::invalidate(name, key); 
			});

			if(!valididty(name, key)) 
			{
				auto func = get(name);
				cached(name, key) = tuple<void>::apply(func, key);
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
			cache_t<T, ARGS...>::invalidate_all(name);
			cache_t<T, ARGS...>::set(name, [value](ARGS... args) { return value; });
		}
	};
	
	template<typename T, typename... Args>
	std::map<std::string, std::map<std::tuple<Args...>, std::vector<std::function<void()>>>> cache_t<T,Args...>::invalidators;

	template<typename T, typename... Args>
	std::map<std::string, std::map<std::tuple<Args...>, bool>> cache_t<T,Args...>::valididty_map;

	template <typename T = bool, typename... ARGS>
	cache_t<T,ARGS...> cache(const std::string& name) 
	{
		return cache_t<T, ARGS...>(name);
	}

	template <typename T = bool, typename... ARGS>
	cache_t<T,ARGS...> cache(const std::string& name, ARGS... args) 
	{
		return cache_t<T, ARGS...>(name, args...);
	}
}
}


#endif