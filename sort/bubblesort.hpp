#ifndef __PG_BUBBLESORT__
#define __PG_BUBBLESORT__

#include <vector>
#include <utility>

namespace pg
{
	class bubblesort
	{
	public:
		template<typename T, typename CMP = std::less<T>>
		void operator()(std::vector<T>& dataset) const
		{
			const CMP cmp;

			for (bool has_swap = true; has_swap;)
			{
				has_swap = false;

				for (int i = 1; i < dataset.size(); i++)
				{
					if (cmp(dataset[i], dataset[i - 1]))
					{
						std::swap(dataset[i], dataset[i - 1]);
						has_swap = true;
					}
				}
			}
		}
	};
}

#endif // __PG_BUBBLESORT__
