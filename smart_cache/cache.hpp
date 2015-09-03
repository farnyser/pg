#ifndef __PG_CACHE__
#define __PG_CACHE__

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <functional>

namespace pg
{
namespace cache
{
	std::stack<std::function<void()>> context_invalidator;

	template<typename T = void, typename... ARGS>
	class cache
	{
	private:
		std::string name;
	
	public:
		cache(const std::string& n) : name(n)
		{
		};

		template<typename... Args>
		bool valid(Args... args) const 
		{
			return cache<T, Args...>::valididty(name, std::make_tuple(args...));
		}

		template<typename... Args>
		T operator()(Args... args) const
		{
			return cache<T, Args...>::getValue(name, args...);
		};

		template<typename... Args>
		T cached(Args... args) const
		{
			return cache<T, Args...>::getCachedValue(name, args...);
		};

		cache& operator=(T value) 
		{
			setValue(name, value);
			return *this;
		};
		
		template<typename... Args>
		cache& operator=(const std::function<T(Args...)>& function) 
		{
			cache<T, Args...>::set(name, function);
			return *this;
		};

		cache& operator=(const std::function<T(ARGS...)>& function) 
		{
			set(name, function);
			return *this;
		};

	private:
		template<typename To, typename... ARGSo> 
		friend class cache;

		typedef std::function<T(ARGS...)> function_t;
		typedef std::tuple<ARGS...> key_t;

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

		static T getCachedValue(const std::string& name, ARGS... args)
		{
			const key_t key(args...);
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

#include "cache.helpers.hpp"

#endif