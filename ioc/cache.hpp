#ifndef __IOC_CACHE_HPP__
#define __IOC_CACHE_HPP__

	#include "ioc.hpp"

	template <typename T, typename K>
	typename T::function::result_type cache(K key, std::function<typename T::function::result_type(void)> f)
	{
		static std::map<K, typename T::function::result_type> cache;
		if(cache.find(key) == cache.end()) 
			cache[key] = f();
		return cache[key];
	}

	template <typename T, typename R>
	void ioc_cache(std::function<R()>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] () {
			return cache<T> (false, [&]() { return function(); });
		};
	}

	template <typename T, typename R, typename P1>
	void ioc_cache(std::function<R(P1)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1) {
			auto key = std::make_tuple(p1);	
			return cache<T> (key, [&]() { return function(p1); });
		};
	}

	template <typename T, typename R, typename P1, typename P2>
	void ioc_cache(std::function<R(P1,P2)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2) {
			auto key = std::make_tuple(p1, p2);
			return cache<T> (key, [&]() { return function(p1, p2); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3>
	void ioc_cache(std::function<R(P1,P2,P3)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3) {
			auto key = std::make_tuple(p1, p2, p3);
			return cache<T> (key, [&]() { return function(p1, p2, p3); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4>
	void ioc_cache(std::function<R(P1,P2,P3,P4)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4) {
			auto key = std::make_tuple(p1, p2, p3, p4);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5>
	void ioc_cache(std::function<R(P1,P2,P3,P4,P5)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			auto key = std::make_tuple(p1, p2, p3, p4, p5);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4, p5); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6>
	void ioc_cache(std::function<R(P1,P2,P3,P4,P5,P6)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			auto key = std::make_tuple(p1, p2, p3, p4, p5, p6);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4, p5, p6); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7>
	void ioc_cache(std::function<R(P1,P2,P3,P4,P5,P6,P7)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			auto key = std::make_tuple(p1, p2, p3, p4, p5, p6, p7);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4, p5, p6, p7); });
		};
	}

	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8>
	void ioc_cache(std::function<R(P1,P2,P3,P4,P5,P6,P7,P8)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
			auto key = std::make_tuple(p1, p2, p3, p4, p5, p6, p7, p8);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4, p5, p6, p7, p8); });
		};
	}
	
	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9>
	void ioc_cache(std::function<R(P1,P2,P3,P4,P5,P6,P7,P8,P9)>)
	{
		typename T::function function = ioc<T>(); 
		ioc<T>() = [function] (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) {
			auto key = std::make_tuple(p1, p2, p3, p4, p5, p6, p7, p8, p9);
			return cache<T> (key, [&]() { return function(p1, p2, p3, p4, p5, p6, p7, p8, p9); });
		};
	}

	#define cached(INAME) ioc_cache<INAME>(INAME::function())

#endif