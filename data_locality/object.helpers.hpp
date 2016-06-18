#include "object.macros.hpp"

#define MAP_NAME(m, a, first, ...)           \
  m(a, first)                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP_NAME)()(m, a+1, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP_NAME() MAP_NAME

#define var(i, A) auto& A() { return get<i>(); };
#define names(...) struct Index { enum { __VA_ARGS__ }; }; EVAL2(MAP_NAME(var, 0, __VA_ARGS__))

#define declare_object(NAME, a, b) \
	namespace pg { \
	namespace object { \
		namespace generated { struct NAME {}; } \
		template <> struct Data<generated::NAME> : public Data<EXPAND b> \
		{ \
			names(EXPAND a) \
			template <typename... ARGS> Data(ARGS... args) : Data<EXPAND b>(args...) {} \
		}; \
	\
		template <> struct Collection<Data<generated::NAME>> : public Collection<Data<generated::NAME>, true> \
		{ \
			names(EXPAND a) \
		}; \
	\
		template <> struct Wrapper<Data<generated::NAME>> : public Wrapper<Data<generated::NAME>, true> \
		{ \
			names(EXPAND a) \
			\
			Wrapper(Wrapper<Data<generated::NAME>, true>& w) \
			: Wrapper<Data<generated::NAME>, true>(w.offset, w.collection) {} \
		}; \
	} \
	} \
	using NAME = pg::object::Data<pg::object::generated::NAME>;
