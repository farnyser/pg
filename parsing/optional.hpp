#ifndef __PG_OPTIONAL_HPP__
#define __PG_OPTIONAL_HPP__

#include <stdexcept>

namespace pg 
{
	namespace utils
	{
		template <typename T>
		class Optional 
		{
			T value;
			bool has_value;

			public:
			Optional() : has_value(false) {}
			Optional(T&& value) : value(value), has_value(true) {}

			T& Value()
			{
				if(!has_value)
					throw std::runtime_error("Optional has no value!");

				return value;
			}

			const T& Value() const 
			{
				return const_cast<Optional<T*>>(this)->Value();			
			}

			T* operator->() 
			{
				if(!has_value)
					throw std::runtime_error("Optional has no value!");

				return &value;
			}

			const T* operator->() const 
			{
				return const_cast<Optional<T>*>(this)->operator->();			
			}

			bool HasValue() const noexcept { return has_value; }
		};
	}
}

#endif /* end of include guard: __PG_OPTIONAL_HPP__ */
