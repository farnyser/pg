Aspect
======

Summary
--------

The basic idea is to increase modularity by separating the core implementation from cross-cutting concerns (logging, authentication, persistence, cache mechanism..).

How can this be done ?
-------------------

Using aspect-oriented programming, its possible to inject code to be executed before/after other function calls.  
In C++ world, it's impossible to do for raw-function unless using some external pre-compiler tools. 
However, we can play around with std::function.

Function overloading
--------------------

A simple way to achieve a form of AOP is to use function composition. 
If you take `f(x)` and want to add some `g()` behavior before every call to `f`, we can create a new function `h(x) = g() then f(x)`.
If you want that to be the new default behavior when calling `f`, it's just a matter of naming/overloading: `f(x) = g() then f_old(x)`.
Of course you can achieve 'after' or 'around' call the same way as this 'before' example.

```C++
f(x) = g(); f(x); 	// before call
f(x) = g(f(x)); 	// around call
f(x) = f(x); g(); 	// after call
```

Dependency injection
--------------------

I also wanted to put inversion of control in place. A method who want to load an user shouldn't kave to know where the data came from (database, file...).
That's why we use IoC and resolve the `load` function using only it's identifier. So, it doesn't depends on the underlying implementation, but only on the interface.

Implementation helpers
----------------------

Declare the interface: 

```C++
aspect_resolvable(ISquare, int(int));
```

Register an implementation:

```C++
overload<ISquare>([](int x) { return x * x; }); 
```

Call it: 

```C+++
call<ISquare>(5);
```

Register an overload for a specific interface: 

```C++
overload<ISquare>([](const ISquare::function& function, int x) 
{ 
	// overload with logging
	std::cout << ISquare::Name << std::endl;
	
	// call existing implementation
	return function(x);
}); 
```

Register a before or after call:

```C++
before([](const std::string& fct) { std::cout << ">>>" << fct << " is going to be called..." << std::endl; });
after([](const std::string& fct) { std::cout << ">>>" << fct << " has been called !" << std::endl; });
```

Automatic memoization
---------------------

It is possible to use an overload to handle automatic memoization with dependency-based automatic invalidation.
It's as easy as writting this:

```C+++
cached<ISquare>();
```

But the best example imho is fibonacci:

```C+++
cached<IFibo>();
cached<IFibo>(0) = 0;
cached<IFibo>(1) = 1;
```

You can also add reactors (callback that'll be called whenever a new value is added to the cache):

```C++
add_reactor<IFibo>([](int n) { 
	std::cout << "IFibo(" << n << ") has changed" << std::endl;
});
```
