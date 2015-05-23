#ifndef __PG_QUICKSORT__
#define __PG_QUICKSORT__

#include <vector>
#include <thread>
#include <utility>

namespace pg
{
	template<typename T, typename CMP = std::less<T>>
	class quicksort
	{
	private:
		const int nthread;
		const CMP cmp;

	public:
		/***
		Construct the quicksort helper
		@param nthread Number of thread used to execute the algorithm (exact number will be 2^nthread)
		*/
		quicksort(int nthread = 0)
				: nthread(nthread), cmp()
		{

		}

		void operator()(std::vector<T>& dataset) const
		{
			sort(dataset, 0, dataset.size() - 1);
		}

	private:
		int partition(std::vector<T>& dataset, int left, int right, int pivotIndex) const
		{
			const T pivotValue(dataset[pivotIndex]);
			std::swap(dataset[pivotIndex], dataset[right]);  // Move pivot to end

			int storeIndex = left;
			for (int i = left; i < right; i++)
			{
				if (cmp(dataset[i], pivotValue))
				{
					std::swap(dataset[i], dataset[storeIndex]);  // Move pivot to end
					storeIndex = storeIndex + 1;
				}
			}

			std::swap(dataset[right], dataset[storeIndex]);  // Move pivot to its final place
			return storeIndex;
		}

		void sort(std::vector<T>& dataset, int left, int right, int thread_level = 0) const
		{
			if (left < right)
			{
				const int pivotIndex = left + (right - left) / 2;
				const int pivotNewIndex = partition(dataset, left, right, pivotIndex);

				if (thread_level < nthread)
				{
					std::thread thl([&]() {
						sort(dataset, left, pivotNewIndex - 1, thread_level + 1);
					});

					sort(dataset, pivotNewIndex + 1, right, thread_level + 1);
					thl.join();
				}
				else
				{
					sort(dataset, left, pivotNewIndex - 1, thread_level + 1);
					sort(dataset, pivotNewIndex + 1, right, thread_level + 1);
				}
			}
		}
	};
}

#endif // __PG_QUICKSORT__
