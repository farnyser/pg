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
			static std::string Name;
		};
		std::string IBefore::Name { "IBefore" };

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
			static std::string Name;
		};
		std::string IAfter::Name { "IAfter" };

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
		decltype(auto) call(ARGS... args)
		{
			ioc<IBefore>()(T::Name);
			decltype(auto) result = ioc<T>()(args...);
			ioc<IAfter>()(T::Name);

			return result;
		}
	}
}
#endif
