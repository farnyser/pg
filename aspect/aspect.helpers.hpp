#ifndef __ASPECT_HELPERS_HPP__
#define __ASPECT_HELPERS_HPP__

#include <functional>
#include <string>
#include <memory>
#include <type_traits>
#include <map>
#include <vector>
#include <stack>
#include <iostream>

	#define aspect_resolvable(INAME, SIGNATURE) \
		class INAME \
		{ \
		public: \
			typedef std::function< SIGNATURE > function; \
			static std::string Name() { return #INAME; }; \
		};

	#define aspect_before(IMPL) \
		{ pg::aspect::overload<IBefore>([function](const std::string& name) IMPL); }

	#define aspect_after(IMPL) \
		{ pg::aspect::overload<IAfter>([function](const std::string& name) IMPL); }

	#define aspect_register(INAME, IMPL) \
		{ typename INAME::function function = pg::aspect::ioc<INAME>(); pg::aspect::ioc<INAME>() = [&, function] IMPL; }

	#define aspect_extends(INAME, IBASE) \
		{ typename IBASE::function function = pg::aspect::ioc<IBASE>(); pg::aspect::ioc<INAME>() = function; }

#endif
