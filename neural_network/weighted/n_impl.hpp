#ifndef __PG_NN_WEIGHTED_NIMPL__
#define __PG_NN_WEIGHTED_NIMPL__

	#include "../neuron.hpp"
	#include <utility>
	
	namespace pg
	{
		namespace nn
		{
			namespace weighted
			{
				template <typename V, typename W>
				class n_impl : public pg::nn::neuron<std::pair<V, V>, W>
				{
				public:
					typedef typename std::pair<V, V> value;

					void input(const typename pg::nn::neuron<value, W>::in& data) override
					{
						if(this->reset)
							this->result = 0;

						this->result += data.first * data.second;
					}

					typename pg::nn::neuron<value, W>::out output() override
					{
						this->reset = true;
						return this->result;
					}
					
				private:
					typename pg::nn::neuron<value, W>::out result {0};
					bool reset {false};
				};
			}
		}
	}

#endif