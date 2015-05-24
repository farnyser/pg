#ifndef __PG_NN_WEIGHTED_LIMPL__
#define __PG_NN_WEIGHTED_LIMPL__

	#include "../link.hpp"
	#include "n_impl.hpp"
	
	namespace pg
	{
		namespace nn
		{
			namespace weighted
			{
				template <typename V, typename W>
				class l_impl : public pg::nn::link<n_impl<V,W>>
				{
				private:
					typename n_impl<V,W>::shared from, to;
					double weight;

				public:
					l_impl(typename n_impl<V,W>::shared in, typename n_impl<V,W>::shared out, double weight)
					: from(in), to(out), weight(weight)
					{
					}

					virtual typename n_impl<V,W>::shared get_from() const override
					{
						return this->from;
					}

					virtual typename n_impl<V,W>::shared get_to() const override
					{
						return this->to;
					}
					
					virtual typename n_impl<V,W>::in get_data(const typename n_impl<V,W>::out& out) const override
					{
						return std::make_pair(this->weight, out);
					}
				};
			}
		}
	}

#endif