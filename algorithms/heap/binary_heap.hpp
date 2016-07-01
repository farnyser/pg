#ifndef __BINARY_HEAP_HPP__
#define __BINARY_HEAP_HPP__

#include <functional>
#include <vector>

namespace pg
{
	namespace heap
	{
		template <typename T, typename C = std::less<T>>
		class Binary
		{
		private:
			std::vector<T> data;
			C cmp;

		private:
			void PercolateUp(int index)
			{
				while(true)
				{
					if(index == 0)
						return;

					auto parent_index = (index - 1) / 2;
					if(cmp(data[index], data[parent_index]))
						std::swap(data[index], data[parent_index]);
					else
						break;

					index = parent_index;
				}
			}

			void PercolateDown(int index)
			{
				while(true)
				{
					auto childA_index = 2 * index + 1;
					auto childB_index = 2 * index + 2;

					if(childA_index >= data.size())
						return;

					auto child_index = childB_index;
					if(childB_index >= data.size() || cmp(data[childA_index], data[childB_index]))
						child_index = childA_index;

					if(cmp(data[child_index], data[index]))
						std::swap(data[index], data[child_index]);
					else
						break;

					index = child_index;
				}
			}

		public:
			Binary() {}
			Binary(C cmp) : cmp(cmp) {}

			bool Empty() const
			{
					return data.size() == 0;
			}

			T& Peek()
			{
				return data[0];
			}

			void Push(T&& e)
			{
				data.push_back(e);
				PercolateUp(data.size()-1);
			}

			T&& Pop()
			{
				auto value = data[0];

				std::swap(data[0], data[data.size()-1]);
				data.pop_back();

				PercolateDown(0);

				return std::move(value);
			}

			template <typename F>
			void Update(T&& e, F fct)
			{
				for(int i = 0; i < data.size() ; i++)
				{
					if(fct(data[i])) {
						if(cmp(data[i], e)) {
							data[i] = e;
							PercolateDown(i);
						}
						else {
							data[i] = e;
							PercolateUp(i);
						}

						break;
					}
				}
			}

			void Update(T&& e, const T& current)
			{
				return Update(std::move(e), [&](const T& a) {
					return !(cmp(a, current) ^ cmp(current, a));
				});
			}
		};
	}
}

#endif
