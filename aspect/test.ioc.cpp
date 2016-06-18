#include "aspect.hpp"
#include "aspect.helpers.hpp"

#include "../test/assert.hpp"

aspect_resolvable(ISquare, int(int));
aspect_resolvable(IIdentity, int(int));

using pg::aspect::call;

int main(int argc, char **argv) 
{
	test("basic IOC", []() 
	{
		aspect_register(ISquare, (int x) { return x * x; }); 
		assertEquals(4, call<ISquare>(2));
		assertEquals(9, call<ISquare>(3));
	});
	
	test("overload 1-level", []() 
	{
		bool overloadCalled{false};
		aspect_register(IIdentity, (int x) { return x; }); 
		
		assertEquals(false, overloadCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(false, overloadCalled);

		aspect_register(IIdentity, (int x) {
			overloadCalled = true;
			return function(x);
		});
		
		assertEquals(false, overloadCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(true, overloadCalled);
	});	
	
	test("overload 2-level", []() 
	{
		bool overloadCalled1{false}, overloadCalled2{false};
		aspect_register(IIdentity, (int x) { return x; }); 
		
		assertEquals(false, overloadCalled1);
		assertEquals(false, overloadCalled2);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(false, overloadCalled1);
		assertEquals(false, overloadCalled2);

		aspect_register(IIdentity, (int x) {
			overloadCalled1 = true;
			return function(x);
		});

		aspect_register(IIdentity, (int x) {
			overloadCalled2 = true;
			return function(x);
		});
		
		assertEquals(false, overloadCalled1);
		assertEquals(false, overloadCalled1);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(true, overloadCalled2);
		assertEquals(true, overloadCalled2);
	});
	
	test("bypass", []() 
	{
		int originalCalled{0};
		aspect_register(IIdentity, (int x) { originalCalled++; return x; }); 
		
		assertEquals(0, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(1, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(2, originalCalled);

		aspect_register(IIdentity, (int x) {
			return x;
		});
		
		assertEquals(2, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(2, originalCalled);
	});

	return EXIT_SUCCESS;
}