#ifndef __PG_CACHE_OPERATORS__
#define __PG_CACHE_OPERATORS__

#include "cache.hpp"

// operator +

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator+(const std::function<O1(Args...)>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) + o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator+(const pg::cache::cache_t<O1, Args...>& o1, const std::function<O2(Args...)>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) + o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator+(const pg::cache::cache_t<O1, Args...>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) + o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator+(const pg::cache::cache_t<O1, Args...>& o1, const O2& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) + o2;
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator+(const O1& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1 + o2(args...);
	};
};


// operator -

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()-O2())(Args...)> operator-(const std::function<O1(Args...)>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) - o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()-O2())(Args...)> operator-(const pg::cache::cache_t<O1, Args...>& o1, const std::function<O2(Args...)>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) - o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()-O2())(Args...)> operator-(const pg::cache::cache_t<O1, Args...>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) - o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()-O2())(Args...)> operator-(const pg::cache::cache_t<O1, Args...>& o1, const O2& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) - o2;
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()-O2())(Args...)> operator-(const O1& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1 - o2(args...);
	};
};

// operator *

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()*O2())(Args...)> operator*(const std::function<O1(Args...)>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) * o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()*O2())(Args...)> operator*(const pg::cache::cache_t<O1, Args...>& o1, const std::function<O2(Args...)>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) * o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()*O2())(Args...)> operator*(const pg::cache::cache_t<O1, Args...>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) * o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()*O2())(Args...)> operator*(const pg::cache::cache_t<O1, Args...>& o1, const O2& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) * o2;
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()*O2())(Args...)> operator*(const O1& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1 * o2(args...);
	};
};

// operator /

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()+O2())(Args...)> operator/(const std::function<O1(Args...)>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) + o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()/O2())(Args...)> operator/(const pg::cache::cache_t<O1, Args...>& o1, const std::function<O2(Args...)>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) / o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()/O2())(Args...)> operator/(const pg::cache::cache_t<O1, Args...>& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) / o2(args...);
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()/O2())(Args...)> operator/(const pg::cache::cache_t<O1, Args...>& o1, const O2& o2)
{
	return [o1, o2](Args... args) {
		return o1(args...) / o2;
	};
};

template<typename O1, typename O2, typename... Args>
std::function<decltype(O1()/O2())(Args...)> operator/(const O1& o1, const pg::cache::cache_t<O2, Args...>& o2)
{
	return [o1, o2](Args... args) {
		return o1 / o2(args...);
	};
};

#endif