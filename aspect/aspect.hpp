#ifndef __ASPECT_HPP__
#define __ASPECT_HPP__

#include <functional>
#include <string>
#include <memory>
#include <type_traits>
#include <map>
#include <vector>
#include <iostream>

#include "aspect.tpl.hpp"

namespace pg
{
	namespace aspect
	{
		/*
			Function register mechanism
		*/

		template <typename T>
		typename T::function& ioc()
		{
			static typename T:: function function;
			return function;
		}

		template <typename IFACE, typename T, typename... ARGS>
		auto overload(const std::function<T(const std::function<T(ARGS...)>&, ARGS...)>& f)
		{
			auto function = ioc<IFACE>();
			ioc<IFACE>() = [=](auto... args) { return f(function, args...); };
		}

		template <typename IFACE, typename T, typename... ARGS>
		auto overload(const std::function<T(ARGS...)>& f)
		{
			ioc<IFACE>() = f;
		}

		template <typename IFACE, typename F>
		auto overload(F f)
		{
			overload<IFACE>(make_function(f));
		}

		/*
			Special function called before every normal functions
		*/

		struct IBefore
		{
			typedef std::function<void(const std::string&)> function;
			static std::string Name() { return "IBefore"; };
		};

		template <>
		typename IBefore::function& ioc<IBefore>()
		{
			static typename IBefore:: function function = [](auto args...) {};
			return function;
		}

		template <typename F>
		auto before(F f)
		{
			overload<IBefore>(make_function(f));
		}

		/*
			Special function called after every normal functions
		*/

		struct IAfter
		{
			typedef std::function<void(const std::string&)> function;
			static std::string Name(){ return "IAfter"; };
		};

		template <>
		typename IAfter::function& ioc<IAfter>()
		{
			static typename IAfter:: function function = [](auto args...) {};
			return function;
		}

		template <typename F>
		auto after(F f)
		{
			overload<IAfter>(make_function(f));
		}

		/*
			Call a registered function
		*/

		template <typename T, typename... ARGS>
		typename T::function::result_type call(ARGS&&... args)
		{
			struct Callback {
				Callback() { ioc<IBefore>()(T::Name()); }
				~Callback() { ioc<IAfter>()(T::Name()); }
			};

			Callback scope;
			return ioc<T>()(std::forward<ARGS>(args)...);
		}
	}
}
#endif
