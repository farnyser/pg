Data Locality
=============

Summary
-------

Object Oriented Programming with cache efficient data layout.

What's the problem ?
--------------------

Let's use a simple example of a Point. If we want to put a point on a surface, we need it's coordinates, and maybe even a name or something. Maybe we'll go crazy and put several point in there.

```C++
struct Point
{
	double x;
	double y;
	std::string name;
}

using Points = std::vector<Point>;
```

These three elements are next to each other in memory. This is optimized for when you need to work on a Point has a whole element (like serialization).

However, often that's not the main scenario. Actualy, most of the time we work on each sub-element independently.

So, what would be the most efficient memory layout to sort Point based on their X coordinate ?

So what can we do ?
-------------------

If we store each element independently, we can play with memory and do whatever we want.
What happen if we replace our Points collection with three collections ? (One per component).

```C++
struct Points
{
	std::vector<double> x;
	std::vector<double> y;
	std::vector<std::string> name;
}
```

What happen is that we get better performance when working on a single component. Finding the point with the lower X can be 2 or 3 times faster than with the unoptimized classical layout.

But it's getting hard to use.

Proposed solution
-----------------

Using some template and macro magic, we can have a nicer interface.
A macro `declare_object` is used to declare and implement all the struct we need for a given type.
The three structs are:

```C++
Point point;
Collection<Point> points;
Wrapper<Point> wrapper(points[0]);
```

`Point` is similar to a POD object, there nothing special about it but the fact that it's using a tuple to store its data.

`Collection<Point>` is simply the N-vector described before with an additional vector of Wrapper<Point>.

`Wrapper<Point>` emulates a Point but is only storing the object's offset in the Collection.

```C++
declare_object(Point, (X, Y, Name), (double, double, std::string));
```

Benchmark
---------

On my i7-3517U laptop, compiled with gcc v5.3.0: `g++ bench.cpp -Wall -march=core-avx-i -Wpsabi -std=c++14 -O3`, this is what I get:

|                                         | vector<Circle> | Collection<Circle> |
|-----------------------------------------|----------------|--------------------|
| Simple access to one component (X)      | 23ms           | 5ms                |
| Range computation using X, Y and Radius | 27ms           | 11ms               |

Similar result have been observed with clang 3.8
