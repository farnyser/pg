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


		template<int...> struct index_tuple{}; 

		template<int I, typename IndexTuple, typename... Types> 
		struct make_indexes_impl; 

		template<int I, int... Indexes, typename T, typename ... Types> 
		struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...> 
		{ 
		    typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type; 
		}; 

		template<int I, int... Indexes> 
		struct make_indexes_impl<I, index_tuple<Indexes...> > 
		{ 
		    typedef index_tuple<Indexes...> type; 
		}; 

		template<typename ... Types> 
		struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...> 
		{};

		template<class Ret, class... Args, int... Indexes > 
		static Ret apply_helper(const std::function<Ret(Args...)>& pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
		{ 
		    return pf( std::forward<Args>( std::get<Indexes>(tup))... ); 
		} 

		template<class Ret, class ... Args> 
		static Ret apply(const std::function<Ret(Args...)>& pf, const std::tuple<Args...>&  tup)
		{
		    return apply_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
		}
	};
}

#endif