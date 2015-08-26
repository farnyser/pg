#ifndef __IOC_HPP__
#define __IOC_HPP__

	#include <functional>
	#include <string>
	#include <memory>
	#include <type_traits>
	#include <map>
	#include <iostream>

	template <typename T>
	typename T::function& ioc()
	{
		static typename T:: function function;
		return function;
	}

	#define resolve(INAME) ioc<INAME>()
	#define register(INAME, IMPL) \
		{ typename INAME::function function = ioc<INAME>(); ioc<INAME>() = [function] IMPL; }
	#define extends(INAME, IBASE) \
		{ typename IBASE::function function = ioc<IBASE>(); ioc<INAME>() = function; }
	#define resolvable(INAME, SIGNATURE) \
		class INAME \
		{ \
		public: \
			typedef std::function< SIGNATURE > function; \
			const virtual std::string name() const { return #INAME; } \
		};

#endif
