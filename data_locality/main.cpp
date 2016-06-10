#include <iostream>
#include <cstdlib>
#include <string>

#include "object.hpp"
#include "object.helpers.hpp"

declare_object(Point, (X, Y), (double, double))
declare_object(Circle, (Center, Radius), (Point, double))

using pg::object::Collection;

void print_circle(auto& circle) 
{
	std::cout << "Circle radius => " << circle.Radius() << std::endl;
	std::cout << "Circle center => (" << circle.Center().X() << ", " << circle.Center().Y() << ")" << std::endl;
}

int main(int argc, char *argv[])
{
	// Create a single object..
	Circle c(Point{0, 3}, 5);

	// Or a collection..
	Collection<Circle> circles;
	
	// Generate some data
	for(int i = 0; i < 10; ++i) 
	{
		Circle circle;
		circle.Radius() = (int)(std::rand() * 10.0 / RAND_MAX);
		circle.Center().X() = (int)(std::rand() * 100.0 / RAND_MAX);
		circle.Center().Y() = (int)(std::rand() * 100.0 / RAND_MAX);
		circles.push_back(circle);
	}
	
	// print small circles
	for(int i = 0; i < 10; ++i)
	{
		// NB: 
		// we are using circles.Radius()[i]
		// instead of classical circles[i].Radius()
		// because all radius are in a vector
		if(circles.Radius()[i] < 5)
			print_circle(circles[i]);
	}
	
	return EXIT_SUCCESS;
};
