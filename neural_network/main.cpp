// g++ -std=c++11 *.cpp ; g++ *.o -o test_nn ; ./test_nn
#include <iostream>
#include <memory>

#include "weighted/weighted.hpp"
#include "network.hpp"

using namespace pg::nn;
using namespace pg::nn::weighted;

int main()
{
	typedef n_impl<double, double> _neuron;
	typedef l_impl<double, double> _link;
	
	std::vector<_neuron::shared> clin;
	std::vector<_neuron::shared> clout;
	std::vector<_link::shared> cll;

	auto a1 = std::make_shared<_neuron>(), a2 = std::make_shared<_neuron>(), a3 = std::make_shared<_neuron>();
	auto b1 = std::make_shared<_neuron>(), b2 = std::make_shared<_neuron>();
	auto c1 = std::make_shared<_neuron>();

	clin.push_back(a1);
	clin.push_back(a2);
	clin.push_back(a3);

	clout.push_back(c1);

	cll.push_back(std::make_shared<_link>(a1, b1, 1));
	cll.push_back(std::make_shared<_link>(a2, b1, 1.5));
	cll.push_back(std::make_shared<_link>(a3, b1, 2));
	cll.push_back(std::make_shared<_link>(a1, b2, 2));
	cll.push_back(std::make_shared<_link>(a2, b2, 1.5));
	cll.push_back(std::make_shared<_link>(a3, b2, 1));
	cll.push_back(std::make_shared<_link>(b1, c1, 1));
	cll.push_back(std::make_shared<_link>(b2, c1, 1));

	pg::nn::network<_neuron, _link> network(clin, cll, clout);

	std::vector<_neuron::in> inputs = {
			_neuron::in(1.0, 1.0),
			_neuron::in(2.0, 1.0),
			_neuron::in(3.0, 1.0)
	};
	network.add_inputs(inputs);

	std::vector<typename _neuron::out> result(1);
	network.get_outputs(result);

	for (auto o : result)
		std::cout << "output: " << o << std::endl;

	return 0;
}