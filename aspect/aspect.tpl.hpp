#ifndef __ASPECT_TEMPLATE_MAGIC_HPP__
#define __ASPECT_TEMPLATE_MAGIC_HPP__

#include <functional>
#include <string>
#include <memory>
#include <type_traits>
#include <map>
#include <vector>
#include <iostream>

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

	
template<size_t N>
struct Apply {
    template<typename F, typename T, typename... A>
    static inline auto apply(F && f, T && t, A &&... a)
        -> decltype(Apply<N-1>::apply(
            ::std::forward<F>(f), ::std::forward<T>(t),
            ::std::get<N-1>(::std::forward<T>(t)), ::std::forward<A>(a)...
        ))
    {
        return Apply<N-1>::apply(::std::forward<F>(f), ::std::forward<T>(t),
            ::std::get<N-1>(::std::forward<T>(t)), ::std::forward<A>(a)...
        );
    }
};

template<>
struct Apply<0> {
    template<typename F, typename T, typename... A>
    static inline auto apply(F && f, T &&, A &&... a)
        -> decltype(::std::forward<F>(f)(::std::forward<A>(a)...))
    {
        return ::std::forward<F>(f)(::std::forward<A>(a)...);
    }
};

template<typename F, typename T>
inline auto apply(F && f, T && t)
    -> decltype(Apply< ::std::tuple_size<
        typename ::std::decay<T>::type
    >::value>::apply(::std::forward<F>(f), ::std::forward<T>(t)))
{
    return Apply< ::std::tuple_size<
        typename ::std::decay<T>::type
    >::value>::apply(::std::forward<F>(f), ::std::forward<T>(t));
}

#endif
