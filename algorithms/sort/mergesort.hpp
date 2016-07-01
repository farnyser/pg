#ifndef __PG_MERGESORT__
#define __PG_MERGESORT__

#include <vector>
#include <thread>

namespace pg
{
	class mergesort
	{
	private:
		const int nthread;

	public:
		/***
		Construct the mergesort helper
		@param nthread Number of thread used to execute the algorithm (exact number will be 2^nthread)
		*/
		mergesort(int nthread = 0)
				: nthread(nthread)
		{

		}

		template<typename T, typename CMP = std::less<T>>
		void operator()(std::vector<T>& dataset, int b = 0, int e = -1, int thread_level = 0)
		{
			if (e == -1)
				e = dataset.size() - 1;

			if (b >= e)
				return;

			const int middle = b + (e - b) / 2;

			if (thread_level < nthread)
			{
				std::thread thl([&]() {
					operator()(dataset, b, middle, thread_level + 1);
				});

				operator()(dataset, middle + 1, e, thread_level + 1);
				thl.join();
			}
			else
			{
				operator()(dataset, b, middle, thread_level + 1);
				operator()(dataset, middle + 1, e, thread_level + 1);
			}

			merge<T, CMP>(dataset, b, middle, e);
		}

	private:
		// not in place
		template<typename T, typename CMP = std::less<T>>
		void merge(std::vector<T>& dataset, int b, int m, int e)
		{
			const CMP cmp;
			int l = b, r = m + 1;
			std::vector<T> tmp;

			while (l <= m and r <= e)
			{
				if (cmp(dataset[l], dataset[r]))
				{
					tmp.push_back(dataset[l]);
					l++;
				}
				else
				{
					tmp.push_back(dataset[r]);
					r++;
				}
			}

			// recopy of left part
			while (l <= m)
			{
				tmp.push_back(dataset[l]);
				l++;
			}

			// recopy of right part
			while (r <= e)
			{
				tmp.push_back(dataset[r]);
				r++;
			}

			for (size_t i = 0; i < tmp.size(); i++)
			{
				dataset[b + i] = tmp[i];
			}
		}
	};
}

#endif // __PG_MERGESORT__
