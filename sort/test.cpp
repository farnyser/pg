#include <cstdlib>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <utility>

#include "bubblesort.hpp"
#include "combsort.hpp"

template <typename T, typename G>
std::vector<T> generate(int size, G generator) 
{
	std::vector<T> result;
	result.resize(size);
	std::generate(result.begin(), result.end(), generator);
	return result;
}

template <typename T>
void print(const T& t) 
{
	for ( auto element : t ) {
		std::cout << element << std::endl;
	}
}

template <typename T>
void assert_sorted(const T& t) 
{
	if ( !std::is_sorted(t.begin(), t.end()) )
		throw std::logic_error("data not sorted");
}

int main(int argc, char **argv)
{	
	const int tries = 50;
	const int size = 1000;
	std::default_random_engine random_engine(time(nullptr));
	std::vector<std::pair<std::string, std::function<void(std::vector<int>&)>>> algorithms = {
		std::make_pair("std::sort", [](std::vector<int>& data){ std:sort(data.begin(), data.end()); }),
		std::make_pair("pg::bubblesort", pg::bubblesort()),
		std::make_pair("pg::combsort(1.3)", pg::combsort(1.3))
	};
	
	for ( auto algo : algorithms ) {
		std::cout << algo.first;
		try {
			for ( int i = 0 ; i < tries ; i++ ) {
				std::vector<int> data = generate<int>(size, random_engine);
				algo.second(data);
				assert_sorted(data);
			}
			
			std:: cout << " -> OK" << std::endl;
		}
		catch ( const std::exception &e ) {
			std:: cout << " -> Failed: " << e.what() << std::endl;
		}
	}
	
	return EXIT_SUCCESS;
}