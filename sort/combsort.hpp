#ifndef __PG_COMBSORT__
#define __PG_COMBSORT__

#include <vector>
#include <utility>

namespace pg {	
	class combsort {
		private:
			const float shrinkf;
		
		public:		
			combsort(const float shrinkf = 1.3) 
				: shrinkf(shrinkf) {
			}
		
			template <typename T, typename CMP = std::less<T>>
			void operator()(std::vector<T> &dataset) const {
				sort<T,CMP>(dataset, 0, dataset.size());
			}
			
			template <typename T, typename CMP = std::less<T>>
			inline void sort(std::vector<T> &dataset, const int begin, const int end) const {
				const CMP cmp;
				
				int gap = end-begin;
				
				for ( bool has_swap = true ; gap > 1 || has_swap ;  ) {
					has_swap = false;
					       
					if (gap > 1) {
						gap = gap / shrinkf;
					}
		
					for ( int i = begin ; gap+i < end ; i++ ) {
						if ( cmp(dataset[i+gap], dataset[i]) ) {
							std::swap(dataset[i], dataset[i+gap]);
							has_swap = true;
						}
					}
				}
			}
	};
}

#endif // __PG_COMBSORT__
