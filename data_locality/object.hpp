#ifndef __PG_OBJECT__
#define __PG_OBJECT__

#include <tuple>
#include <vector>

namespace pg
{
namespace object
{
	template<size_t I = 0, typename Func, typename ...Ts, typename ...TsBis>
	typename std::enable_if<I == sizeof...(Ts)>::type
	for_each_in_tuple_pair(Func func, std::tuple<Ts...> & tpl, std::tuple<TsBis...> & tplBis) 
	{}

	template<size_t I = 0, typename Func, typename ...Ts, typename ...TsBis>
	typename std::enable_if<I < sizeof...(Ts)>::type
	for_each_in_tuple_pair(Func func, std::tuple<Ts...> & tpl, std::tuple<TsBis...> & tplBis) 
	{
		func(std::get<I>(tpl), std::get<I>(tplBis));
		for_each_in_tuple_pair<I + 1>(func, tpl, tplBis);
	}


	template<size_t I = 0, typename Func, typename ...Ts, typename ...TsBis>
	typename std::enable_if<I == sizeof...(Ts)>::type
	for_each_in_tuple_pair(Func func, std::tuple<Ts...> & tpl, const std::tuple<TsBis...> & tplBis) 
	{}

	template<size_t I = 0, typename Func, typename ...Ts, typename ...TsBis>
	typename std::enable_if<I < sizeof...(Ts)>::type
	for_each_in_tuple_pair(Func func, std::tuple<Ts...> & tpl, const std::tuple<TsBis...> & tplBis) 
	{
		func(std::get<I>(tpl), std::get<I>(tplBis));
		for_each_in_tuple_pair<I + 1>(func, tpl, tplBis);
	}

	template<typename... T>
	struct Data 
	{
		typedef std::tuple<T...> type;
		
		std::tuple<T...> data;
		
		template <typename... ARGS> 
		Data(ARGS... args) : data(args...) {} 

		template <int N>
		auto& get() {
			return std::get<N>(data);
		}
		
		template <int N>
		const auto& get() const {
			return std::get<N>(data);
		}
	};
	
	template <typename T, bool INTERNAL = false>
	struct Collection;
	
	template <typename T, bool INTERNAL = false>
	struct Wrapper
	{
		int offset;
		Collection<T, true>& collection;
		
		Wrapper(int offset, Collection<T, true>& collection) 
		: offset(offset), collection(collection) {}
		
		template <int N, typename Enable = void>
		auto& get() {
			return collection.get<N>()[offset];
		}
		
		operator T() {
			T result;
			for_each_in_tuple_pair([&](auto &res, auto& orig){ res = orig[offset]; }, result.data, collection.data);
			return result;
		}
	};
		
	template <typename T> 
	struct CollectionBridge 
	{
		using type = std::vector<T>;
	};
	
	template <typename T> 
	struct CollectionBridge<Data<T>> 
	{
		using type = Collection<Data<T>, false>;
	};
		
	template <typename T, bool INTERNAL>
	struct Collection
	{
		private:
		template<size_t... Is>
		static std::tuple<
			typename CollectionBridge<
				std::tuple_element_t<Is, typename T::type>
			>::type 
		...>
			helper(std::index_sequence<Is...>);
		using tuple_type = decltype(helper(std::make_index_sequence<std::tuple_size<typename T::type>::value>()));

		tuple_type data;
		std::vector<Wrapper<T,false>> wrappers;
		
		public:
		template <int N>
		auto& get() {
			return std::get<N>(data);
		}
		
		template <int N>
		auto& get() const {
			return std::get<N>(data);
		}
		
		auto& operator[](int i) {
			return wrappers[i];
		}
		
		auto begin() { return wrappers.begin(); }
		auto begin() const { return wrappers.begin(); }
		auto rbegin() { return wrappers.rbegin(); }
		auto rbegin() const { return wrappers.rbegin(); }
		auto end() { return wrappers.end(); }
		auto end() const { return wrappers.end(); }
		auto rend() { return wrappers.rend(); }
		auto rend() const { return wrappers.rend(); }
		
		void push_back(const T& t) {
			for_each_in_tuple_pair([](auto &cl, auto& td) { cl.push_back(td); }, data, t.data);
			
			Wrapper<T, true> w(wrappers.size(), *this);
			wrappers.push_back(Wrapper<T, false>( w ));
		}
		
		auto size() const {
			return get<0>().size();
		}
		
		friend struct Wrapper<T>;
	};
}
}

#endif