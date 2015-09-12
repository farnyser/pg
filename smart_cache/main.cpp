#include <iostream>
#include <string>
#include <sstream>
#include "cache.hpp"
#include "cache.operators.hpp"

using pg::cache::cache;

template <typename T1, typename T2>
void assertEquals(T1 t1, T2 t2)	
{
	if(!(t1 == t2)) 
	{
		std::stringstream ss;
		ss << "Assertion failed: ";
		ss << "Expected:" << t2;
		ss << ", Got:" << t1;
		throw std::runtime_error(ss.str());
	}
}

void ComputeFibonacciSample()
{
	// used only to check cache mechanism is working
	int count_call = 0;

	// recursive definition
	cache("fibonacci", 0) = 0;
	cache("fibonacci", 1) = 1;
	cache("fibonacci") = make_function([&](int n) 
	{
		count_call++;
		return cache<int>("fibonacci")(n-1) + cache<int>("fibonacci")(n-2);
	});

	assertEquals(cache<int>("fibonacci")(10), 55);
	assertEquals(count_call, 9);

	assertEquals(cache<int>("fibonacci")(25), 75025);
	assertEquals(count_call, 24);

	assertEquals(cache<int>("fibonacci")(16), 987);
	assertEquals(count_call, 24);

	// assert cache invalidation works
	cache("fibonacci", 0) = 100;
	assertEquals(cache<int>("fibonacci").valid(16), false);
}

void ComputeBasketSample()
{
	cache("count", "banana"_s) = 10;
	cache("count", "apple"_s) = 5;

	cache("price", "banana"_s) = 2.3;
	cache("price", "apple"_s) = 1.0;

	cache("sum") = cache<double,std::string>("price") * cache<int,std::string>("count");

	cache("total") = make_function([](){
		return cache<double>("sum")("apple"_s) + cache<double>("sum")("banana"_s);
	});

	assertEquals(cache<double>("price")("banana"_s), 2.3);
	assertEquals(cache<double>("price")("apple"_s), 1.0);
	assertEquals(cache<double>("sum")("banana"_s), 23);	
	assertEquals(cache<double>("sum")("apple"_s), 5);
	assertEquals(cache<double>("total")(), 28);

	// invalidate apple price and recompute
	cache("price", "apple"_s) = 2.5;
	assertEquals(cache<double>("total")(), 35.5);
}

int main(int argc, char *argv[])
{
	ComputeFibonacciSample();
	ComputeBasketSample();

	return 0;
};
