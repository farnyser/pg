#ifndef __DDH_HPP__
#define __DDH_HPP__

	#include <string>
	#include <vector>
	#include <map>

	namespace pg
	{	
		// forward declaration
		template <typename C, typename K> class DynamicDataHolder;

		// actual data holder
		template <typename C, typename T, typename K = std::string>
		class DynamicDataType
		{
			std::vector<T> data;
			std::map<K, std::vector<T>> overload;
			friend class DynamicDataHolder<C, K>;
		};

		// data access
		template <typename C, typename K = std::string>
		class DynamicDataHolder
		{
		public:
			template <typename T>
			static int& GetSize()
			{
				static int counter = 0;
				return counter;
			}

			template <typename T>
			static int GetIndex(const std::string& name)
			{
				static std::map<std::string, int> names;

				auto it = names.insert(std::make_pair(name, GetSize<T>()));
				if(it.second)
					return GetSize<T>()++;
				else 
					return it.first->second;
			}

			template <typename T>
			static int Register(const std::string& name)
			{
				auto index = GetIndex<T>(name);
				return index;
			}

			template <typename T>
			std::vector<T>& Get()
			{
				auto self = (C*)this;
				auto& data = self->DynamicDataType<C,T>::data;
				auto size = GetSize<T>();
				if(data.size() < size)
					data.resize(size);
				return data;
			}

			template <typename T>
			std::vector<T>& Get(const K& key)
			{
				auto self = (C*)this;
				auto& data = self->DynamicDataType<C,T>::overload[key];
				auto size = GetSize<T>();
				if(data.size() < size)
					data.resize(size);
				return data;
			}
		};
	}

#endif