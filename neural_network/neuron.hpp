#ifndef __PG_NN_NEURON__
#define __PG_NN_NEURON__

#include <memory>

namespace pg
{
	namespace nn
	{
		template<typename Tin, typename Tout = Tin>
		class neuron
		{
		public:
			typedef Tin in;
			typedef Tout out;
			typedef std::shared_ptr <neuron<Tin, Tout>> shared;

			virtual void input(const Tin& data) = 0;

			virtual Tout output() = 0;

			virtual ~neuron() = default;
		};
	}
}

#endif