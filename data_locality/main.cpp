// Should be compiled with a C++14 compatible compiler
#include <iostream>
#include <cstdlib>
#include <string>

#include "object.hpp"
#include "object.helpers.hpp"

// Create a kind-of struct Point { double X; double Y; std::string Name; };
declare_object(Point, (X, Y, Name), (double, double, std::string));

// Create a kind-of struct Circle { Point Center; double Radius; };
declare_object(Circle, (Center, Radius), (Point, double));

using pg::object::Collection;

auto print_circle = [](auto& circle)
{
	std::cout << "Circle radius => " << circle.Radius() << std::endl;
	std::cout << "Circle center => (" << circle.Center().Name() << ": " << circle.Center().X() << ", " << circle.Center().Y() << ")" << std::endl;
};

int main(int argc, char *argv[])
{
	// Create a single object..
	Circle c(Point{0, 3, "A"}, 5);

	// Or a collection..
	Collection<Circle> circles;

	// Generate some data
	for(int i = 0; i < 10; ++i)
	{
		Circle circle;
		circle.Radius() = (int)(std::rand() * 10.0 / RAND_MAX);
		circle.Center().Name() = 'A' + (int)(std::rand() * 25.0 / RAND_MAX);
		circle.Center().X() = (int)(std::rand() * 100.0 / RAND_MAX);
		circle.Center().Y() = (int)(std::rand() * 100.0 / RAND_MAX);
		circles.push_back(circle);
	}

	std::cout << "Print small circles using optimized memory layout" << std::endl;
	for(int i = 0; i < 10; ++i)
	{
		// NB:
		// we are using circles.Radius()[i]
		// instead of classical circles[i].Radius()
		// because all radius are in a vector
		if(circles.Radius()[i] < 5)
			print_circle(circles[i]);
	}

	std::cout << "Print large circles using wrapper (pseudo object)" << std::endl;
	for(auto w : circles)
	{
		if(w.Radius() >= 5)
			print_circle(w);
	}

	return EXIT_SUCCESS;
};
