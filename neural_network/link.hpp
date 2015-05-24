#ifndef __PG_NN_LINK__
#define __PG_NN_LINK__

#include <memory>

namespace pg
{
	namespace nn
	{
		template<typename Tn>
		class link
		{
		public:
			typedef std::shared_ptr<link<Tn>> shared;

			/**
			* Get neuron at start point of this link
			*/
			virtual typename Tn::shared get_from() const = 0;

			/**
			* Get neuron at end point of this link
			*/
			virtual typename Tn::shared get_to() const = 0;

			/**
			* Convert an output value into an input value
			*
			* Output can be used directly as input in some simple cases.
			* For example, this convert method can be used to apply a weight distinct for each links.
			*/
			virtual typename Tn::in get_data(const typename Tn::out&) const = 0;

			virtual ~link() = default;
		};
	}
}

#endif