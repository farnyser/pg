// g++ -std=c++11 main.cpp -o ioc_test && ./ioc_test
#include "ioc.hpp"
#include "cache.hpp"

resolvable(IIntGetter, int(const std::string&));
resolvable(ICachedIntGetter, int(const std::string&));
resolvable(IGetAge, int());
resolvable(IGetSize, int());
 
void setup()
{
	// Ask user for an int, and returns it
	register(IIntGetter, (const std::string& str) { 
		int result;
		std::cout << str << std::endl;
		std::cin >> result;
		return result;
	});
	
	// ICachedIntGetter is a copy of IIntGetter
	extends(ICachedIntGetter, IIntGetter);
	
	// Every call to ICachedIntGetter with identical parameters
	// will be fetched from a cache
	cached(ICachedIntGetter);
	
	// Ask user for it's age
	register(IGetAge, () {
		return resolve(ICachedIntGetter)("Please enter your age: ");
	});
  
	// Check age validity
	register(IGetAge, () {
		auto result = function();
		if(result < 0 || result > 130) 
			throw std::logic_error("Age should be between 0 and 130");
		return result;
	});
	
	// Ask user for it's size
	register(IGetSize, () {
		return resolve(ICachedIntGetter)("Please enter your size: ");
	});	
}

int main(int argc, char **argv)
{
	// Prepare IoC
	setup();
	
	// Get the user's size
	resolve(IGetSize)();
	
	// Get the user's age 
	resolve(IGetAge)();
	
	// Here, IGetSize and IGetAge are taken from cache 
	std::cout << "your size is : " << resolve(IGetSize)() << std::endl;
	std::cout << "your age is : " << resolve(IGetAge)() << std::endl;
 	
 	return 0;
}
