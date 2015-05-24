#ifndef __PG_NN_NETWORK__
#define __PG_NN_NETWORK__

#include <memory>
#include <vector>

#include "link.hpp"

namespace pg
{
	namespace nn
	{
		template<typename Tn, typename Tl>
		class network
		{
		private:
			std::vector<typename Tn::shared> inputs;
			std::vector<typename Tl::shared> links;
			std::vector<typename Tn::shared> outputs;

			void compute()
			{
				for (auto l : links)
				{
					auto from = l->get_from();
					auto to = l->get_to();
					to->input(l->get_data(from->output()));
				}
			}

		public:
			template<typename Tc_n_in, typename Tc_l, typename Tc_n_out>
			network(Tc_n_in inputs, Tc_l links, Tc_n_out outputs)
			: inputs(inputs), links(links), outputs(outputs)
			{
			}

			template<typename Tc_v_in>
			void add_inputs(Tc_v_in& values)
			{
				int i = 0;
				for (auto in : values)
					inputs.at(i++)->input(in);
			}

			template<typename Tc_v_out>
			void get_outputs(Tc_v_out& out)
			{
				compute();

				int i = 0;
				for (auto o : outputs)
					out.at(i++) = o->output();
			}
		};
	}
}

#endif