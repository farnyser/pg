#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <chrono>
#include <algorithm>

#include "object.hpp"
#include "object.helpers.hpp"

declare_object(Point, (X, Y, Name), (double, double, std::string))
declare_object(Circle, (Center, Radius), (Point, double))

using pg::object::Collection;


template <typename F>
void time(F f, const std::string& label, int x = 1)
{
	using namespace std;
	using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for(int i = 0 ; i < x ; i++) {
		f();
    }
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << label << std::setprecision(3) << duration/x/1000.0 << " ms" << endl;
}

int main(int argc, char *argv[])
{
	constexpr int N = 7000000;
	constexpr int NB_REPETITION = 3;
	
	Collection<Circle> circles;
	std::vector<Circle> v_circles;
	
	// generating some random sample
	for(int i = 0; i < N ; ++i) 
	{
		Circle circle;
		circle.Radius() = (int)(std::rand() * 10.0 / RAND_MAX);
		circle.Center().Name() = 'A' + (int)(std::rand() * 25.0 / RAND_MAX);
		circle.Center().X() = (int)(std::rand() * 100.0 / RAND_MAX);
		circle.Center().Y() = (int)(std::rand() * 100.0 / RAND_MAX);

		// Collection
		circles.push_back(circle);
	
		// std::vector
		v_circles.push_back(circle);
	}
	
	Point target;
	target.X() = 25;
	target.Y() = 25;
	
	auto TargetInRange = [&target](double x, double y, double radius) {
		auto sqDistance = (x - target.X()) * (x - target.X())
						- (y - target.Y()) * (y - target.Y());
		return sqDistance < radius * radius;
	};
	
	// first example : count how many circles are on the left of target ?
	
	time([&]() {
		long count = 0;
		for(int i = 0; i < N ; ++i) 
			count += v_circles[i].Center().X() < target.X();
		std::cout << "Number of fighter on left: " << count << std::endl;
	}, "vector<Circle> -> ", NB_REPETITION);
	
	time([&]() {
		long count = 0;
		for(int i = 0; i < N ; ++i) 
			count += circles.Center().X()[i] < target.X();
		std::cout << "Number of fighter on left: " << count << std::endl;
	}, "Collection<Circle> -> ", NB_REPETITION);
	
	// second example : count how many circles include target
	
	time([&]() {
		long count = 0;
		for(int i = 0; i < N ; ++i) 
			count += TargetInRange(v_circles[i].Center().X(), v_circles[i].Center().Y(), v_circles[i].Radius());
		std::cout << "Number of fighter in range: " << count << std::endl;
	}, "vector<Circle> -> ", NB_REPETITION);
		
	time([&]() {
		long count = 0;
		for(int i = 0; i < N ; ++i) 
			count += TargetInRange(circles.Center().X()[i], circles.Center().Y()[i], circles.Radius()[i]);
		std::cout << "Number of fighter in range: " << count << std::endl;
	}, "Colletion<Circle> -> ", NB_REPETITION);	
		
	
	return EXIT_SUCCESS;
};
