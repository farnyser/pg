#include <iostream>
#include <string>
#include "cache.hpp"
#include "cache.operators.hpp"

int main(int argc, char *argv[])
{
	using pg::cache::cache;
		
	cache<int>("apple_count") = 10;
	cache<int>("banana_count") = 5;
	cache<double>("apple_price") = 2.3;
	cache<double>("banana_price") = 1;

	cache<int, std::string>("count") = [](std::string str)
	{
		std::cout << " - computing count for " << str << std::endl;

		if(str == "banana")
			return cache<int>("banana_count")();
		else if(str == "apple")
			return cache<int>("apple_count")();
		else 
			return 0;
	};

	cache<double, std::string>("price") = [](std::string str)
	{
		std::cout << " - computing price for " << str << std::endl;

		if(str == "banana")
			return cache<double>("banana_price")();
		else if(str == "apple")
			return cache<double>("apple_price")();
		else 
			return 0.0;
	};

	cache<double>("sum") = cache<double,std::string>("price") * cache<int,std::string>("count");

	auto sum = cache<double>("sum");
	
	std::cout << "banana" << " => " << sum("banana"_s) << std::endl;
	std::cout << "banana" << " => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;

	std::cout << "invalidate price for " << "banana" << std::endl;
	cache<double>("banana_price") = 0.5;

	std::cout << "banana" << " => " << (sum.valid("banana"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "banana" << " => old value => " << sum.cached("banana"_s) << std::endl;
	std::cout << "banana" << " => new value => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << (sum.valid("apple"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;

	std::cout << "invalidate count for all" << std::endl;
	cache<int, std::string>("count") = 1;

	std::cout << "banana" << " => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;
 	
 	std::cout << "invalidate sum for all (adding fx)" << std::endl;
	cache<double>("sum") = 1.10 * cache<double,std::string>("price") * cache<int,std::string>("count");

	std::cout << "banana" << " => " << (sum.valid("banana"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "apple" << " => " << (sum.valid("apple"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "banana" << " => new value => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => new value => " << sum("apple"_s) << std::endl;

	return 0;
};
