#ifndef __PG_JSON__
#define __PG_JSON__

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <type_traits>

namespace pg
{
namespace json
{
template <typename T> struct is_collection { static const bool value = false; };
template <typename T> struct is_collection< std::vector<T> > { static const bool value = true; };
template <typename T> struct is_collection< std::list<T> > { static const bool value = true; };
template <typename T> struct is_collection< std::set<T> > { static const bool value = true; };

template <typename T>
class ISerializable
{
};

template <typename S, typename T, typename... ARGS> 
struct Serializable 
{
	const Serializable<S, T> first;
	const Serializable<ARGS...> next;

	Serializable(const char* name, const T value, ARGS... args)
	 : first(name,value), next(args...)
	 {}
};


template <typename S, typename T> 
struct Serializable <S,T>
{
	const char* name;
	const T value;

	Serializable(const char* name, const T value)
	 : name(name), value(value)
	 {}
};


template <typename T, typename... ARGS> 
Serializable<const char*, T, ARGS...> make_serializable(
	const char* name, 
	const T value,
	ARGS... args
)
{
    return Serializable<const char*, T, ARGS...>(name, value, args...);
}


template <typename T> struct is_serializable { static const bool value = false; };
template <typename T> struct is_serializable< Serializable<const char*, T> > { static const bool value = true; };

template <typename T, typename... ARGS> struct is_serializable_more { static const bool value = false; };
template <typename T, typename... ARGS> struct is_serializable_more< Serializable<const char*, T, const char*, ARGS...> > { static const bool value = true; };


template <typename T, 
	typename std::enable_if<
    	!std::is_base_of<ISerializable<T>,T>::value
    	&& !is_serializable<T>::value
    	&& !is_serializable_more<T>::value
    	&& !is_collection<T>::value
	>::type* = nullptr
> 
void serialize(std::ostream& os, const T& data)
{
	os << data;
}

template <typename T,
	typename std::enable_if<
    	std::is_base_of<ISerializable<T>,T>::value
	>::type* = nullptr
> 
void serialize(std::ostream& os, const ISerializable<T>& data)
{
    os << "{" ;
    serialize(os, ((T&)data).make_serializable());
    os << "}";
}

#include "json.basics.hpp"

template <typename T> 
void serialize(std::ostream& os, const Serializable<const char*, T>& data)
{
    os << "\"" << data.name << "\":";
    serialize(os, *data.value);
}

template <typename T, typename...ARGS> 
void serialize(std::ostream& os, const Serializable<const char*, T, ARGS...>& data)
{
    serialize(os, data.first);
    os << ",";
    serialize(os, data.next);
}

}
}

#define SERIALIZABLE(...) \
	auto make_serializable() { \
		return pg::json::make_serializable( \
			__VA_ARGS__ \
		); \
	}
#define _(N) #N, &N 
#define $(N, V) N, &V

#endif