#ifndef __PG_CACHE_HELPERS__
#define __PG_CACHE_HELPERS__

#include "cache.hpp"

template<typename T> struct remove_class { };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...)> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

template<typename T>
struct get_signature_impl { using type = typename remove_class<
    decltype(&std::remove_reference<T>::type::operator())>::type; };
template<typename R, typename... A>
struct get_signature_impl<R(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
template<typename T> using get_signature = typename get_signature_impl<T>::type;

template<typename F> using make_function_type = std::function<get_signature<F>>;
template<typename F> make_function_type<F> make_function(F &&f) {
    return make_function_type<F>(std::forward<F>(f)); }

std::string operator"" _s(const char* str, size_t size)
{
    return std::string(str);
};

namespace pg 
{
	template<typename... ARGS>
	struct tuple 
	{
		template<typename... Args> 
		static 			
		typename std::enable_if<
			std::is_same<std::tuple<Args...>, std::tuple<ARGS...>>::value 
			, std::tuple<ARGS...>
		>::type
		make(Args... args)
		{
			return std::make_tuple(args...);
		}

		template<typename... Args> 
		static 			
		typename std::enable_if<
			!std::is_same<std::tuple<Args...>, std::tuple<ARGS...>>::value 
			, std::tuple<ARGS...>
		>::type
		make(Args... args)
		{
			return std::tuple<ARGS...>();
		}

		template<typename... Args> 
		static 			
		typename std::enable_if<
			std::is_same<std::tuple<Args...>, std::tuple<ARGS...>>::value 
			, bool
		>::type
		valid(Args... args)
		{
			return true;
		}

		template<typename... Args> 
		static 			
		typename std::enable_if<
			!std::is_same<std::tuple<Args...>, std::tuple<ARGS...>>::value 
			, bool
		>::type
		valid(Args... args)
		{
			return false;
		}
	};
}

#endif