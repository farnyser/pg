#include <cstdlib>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <utility>
#include <chrono>

#include "bubblesort.hpp"
#include "combsort.hpp"
#include "mergesort.hpp"
#include "quicksort.hpp"

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
	if ( !std::is_sorted(t.begin(), t.end()) ) {
		print(t);
		throw std::logic_error("data not sorted");
	}
}

int main(int argc, char **argv)
{	
	const int tries = 2;
	const int size = 100000;
	std::default_random_engine random_engine(time(nullptr));
	std::vector<std::pair<std::string, std::function<void(std::vector<int>&)>>> algorithms = {
		std::make_pair("std::sort", [](std::vector<int>& data){ std:sort(data.begin(), data.end()); }),
		//~ std::make_pair("pg::bubblesort", pg::bubblesort()),
		std::make_pair("pg::combsort(1.3)", pg::combsort(1.3)),
		std::make_pair("pg::mergesort()", pg::mergesort()),
		std::make_pair("pg::mergesort(2)", pg::mergesort(2)),
		std::make_pair("pg::mergesort(4)", pg::mergesort(4)),
		std::make_pair("pg::quicksort()", pg::quicksort<int>()),
		std::make_pair("pg::quicksort(2)", pg::quicksort<int>(2)),
		std::make_pair("pg::quicksort(4)", pg::quicksort<int>(4)),
	};
	
	for ( auto algo : algorithms ) {
		std::cout << algo.first;
		try {
			auto start = std::chrono::steady_clock::now();
			
			for ( int i = 0 ; i < tries ; i++ ) {
				std::vector<int> data = generate<int>(size, random_engine);
				algo.second(data);
				assert_sorted(data);
			}
			
			auto end = std::chrono::steady_clock::now();
			auto diff = end - start;
			int duration = std::chrono::duration <double, std::milli> (diff).count();
	
			std:: cout << " -> OK in " << duration << " ms" << std::endl;
		}
		catch ( const std::exception &e ) {
			std:: cout << " -> Failed: " << e.what() << std::endl;
		}
	}
	
	return EXIT_SUCCESS;
}