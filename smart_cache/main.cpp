#include <iostream>
#include <string>
#include "cache.hpp"
#include "cache.operators.hpp"

int main(int argc, char *argv[])
{
	using pg::cache::cache;
	
	cache<int>("apple_count") = 10;
	cache<int>("banana_count") = 5;

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
	auto banana_price = cache<double, std::string>("price", "banana"_s);
	banana_price = 2.3;
	cache<double, std::string>("price", "apple"_s) = 1;

	cache<double>("sum") = cache<double,std::string>("price") * cache<int,std::string>("count");

	auto sum = cache<double>("sum");
	
	std::cout << "banana" << " => " << sum("banana"_s) << std::endl;
	std::cout << "banana" << " => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;

	std::cout << "price banana" << " => " << (banana_price.valid() ? "valid" : "to be computed") << std::endl;
	std::cout << "price apple" << " => " << (cache<double>("price").valid("apple"_s) ? "valid" : "to be computed") << std::endl;

	std::cout << "invalidate price for " << "banana" << std::endl;
	banana_price = 0.5;

	std::cout << "banana" << " => " << (sum.valid("banana"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "banana" << " => old value => " << sum.cached("banana"_s) << std::endl;
	std::cout << "banana" << " => new value => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << (sum.valid("apple"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;

	std::cout << "invalidate count for all" << std::endl;
	cache<int, std::string>("count") = 1;

	std::cout << "banana" << " => " << (sum.valid("banana"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "banana" << " => old value => " << sum.cached("banana"_s) << std::endl;
	std::cout << "banana" << " => new value => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => " << (sum.valid("apple"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "apple" << " => old value => " << sum.cached("apple"_s) << std::endl;
	std::cout << "apple" << " => new value => " << sum("apple"_s) << std::endl;
	std::cout << "apple" << " => " << sum("apple"_s) << std::endl;
 	
 	std::cout << "invalidate sum for all (adding fx)" << std::endl;
	cache<double>("sum") = 1.10 * cache<double,std::string>("price") * cache<int,std::string>("count");

	std::cout << "banana" << " => " << (sum.valid("banana"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "apple" << " => " << (sum.valid("apple"_s) ? "valid" : "to be computed") << std::endl;
	std::cout << "banana" << " => new value => " << sum("banana"_s) << std::endl;
	std::cout << "apple" << " => new value => " << sum("apple"_s) << std::endl;


	cache<int, int>("fibo", 0) = 1;
	cache<int, int>("fibo", 1) = 1;
	cache<int, int>("fibo") = [](int n) {
		return cache<int>("fibo")(n-1) + cache<int>("fibo")(n-2);
	};

	std::cout << cache<int>("fibo")(10) << std::endl;
	std::cout << cache<int>("fibo")(14) << std::endl;
	std::cout << cache<int>("fibo")(24) << std::endl;

	return 0;
};
