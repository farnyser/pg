#include "aspect.hpp"
#include "aspect.helpers.hpp"
#include "aspect.cache.hpp"

#include "../test/assert.hpp"

aspect_resolvable(ISquare, int(int));
aspect_resolvable(IIdentity, int(int));
aspect_resolvable(IFibo, int(int));

aspect_resolvable(ICount, int(const std::string&));
aspect_resolvable(IPriceInUSD, double(const std::string&));
aspect_resolvable(IPriceInEUR, double(const std::string&));
aspect_resolvable(IConvertUSDtoEUR, double(double));
aspect_resolvable(IBasketValue, double());

using pg::aspect::call;
using pg::aspect::cached;
using pg::aspect::overload;
using pg::aspect::validity;
using pg::aspect::add_reactor;

int main(int argc, char **argv) 
{
	test("basic cache", []() 
	{
		int originalCalled{0};
		overload<IIdentity>([&](int x) { originalCalled++; return x; }); 
		
		assertEquals(0, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(1, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(2, originalCalled);

		cached<IIdentity>();
		
		originalCalled = 0;
		assertEquals(0, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(1, originalCalled);
		assertEquals(5, call<IIdentity>(5));
		assertEquals(1, originalCalled);
	});
	
	test("manual overload", []() 
	{
		overload<IFibo>([](int n) { return call<IFibo>(n-1) + call<IFibo>(n-2); });
		cached<IFibo>();
		cached<IFibo>(0) = 0;
		cached<IFibo>(1) = 1;
		
		assertEquals(5, call<IFibo>(5));
		assertEquals(55, call<IFibo>(10));

	});
	
	test("dependency", []() 
	{
		assertEquals(55, call<IFibo>(10));
		assertEquals(true, validity<IFibo>(9));

		cached<IFibo>(8) = 0;
		assertEquals(true, validity<IFibo>(8));
		assertEquals(false, validity<IFibo>(9));
		
		assertEquals(13, call<IFibo>(10));
		
		validity<IFibo>(8) = false;
		
		assertEquals(validity<IFibo>(9), false);
		assertEquals(validity<IFibo>(10), false);
	});
	
	test("dependency 2-levels", []() 
	{
		// setup
		overload<IConvertUSDtoEUR>([](double x) { 
			return x * 1.1;
		});
		
		cached<ICount>();
		cached<ICount>(std::string{"apple"}) = 5;
		
		cached<IPriceInUSD>();
		cached<IPriceInUSD>(std::string{"apple"}) = 1.3;
		
		overload<IPriceInEUR>([](const std::string& x) { 
			return call<IConvertUSDtoEUR>(call<IPriceInUSD>(x));
		});
		cached<IPriceInEUR>();
		
		overload<IBasketValue>([](){
			return call<IPriceInEUR>("apple") * call<ICount>(std::string("apple"));
		});
		cached<IBasketValue>();
		
		assertEquals(7.15, call<IBasketValue>());
		
		// change price
		cached<IPriceInUSD>(std::string{"apple"}) = 0.7;
		
		assertEquals(true, validity<IPriceInUSD>(std::string{"apple"}));
		assertEquals(false, validity<IPriceInEUR>(std::string{"apple"}));
		assertEquals(true, validity<ICount>(std::string{"apple"}));
		assertEquals(false, validity<IBasketValue>());
		call<IBasketValue>();
		assertEquals(true, validity<IPriceInEUR>(std::string{"apple"}));
		assertEquals(true, validity<IBasketValue>());
	});

	return EXIT_SUCCESS;
}