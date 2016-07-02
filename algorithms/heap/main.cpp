#include <iostream>
#include <utility>
#include <algorithm>
#include <queue>
#include <vector>

#include "binary_heap.hpp"
#include "pairing_heap.hpp"

#include "../../test/assert.hpp"

template <typename T>
class StdWrapper
{
	std::priority_queue<T, std::vector<T>, std::greater<T>> data;
public:
	void Push(T e) { data.push(e); }
	T Pop() { auto v = data.top(); data.pop(); return v; }
	void Update(T a, T b) {}
	bool Empty() { return data.empty(); }
};

int main(int argc, char **argv)
{
	// using PriorityQueue = pg::heap::Binary<int>;
	using PriorityQueue = pg::heap::Pairing<int>;
	// using PriorityQueue = StdWrapper<int>;

	test("Basic Push/Pop", []()
	{
		PriorityQueue pq;

		pq.Push(10);
		pq.Push(5);
		pq.Push(8);
		pq.Push(12);
		pq.Push(1);
		pq.Push(90);
		pq.Push(9);

		assertEquals(1, pq.Pop());
		assertEquals(5, pq.Pop());
		assertEquals(8, pq.Pop());
		assertEquals(9, pq.Pop());
		assertEquals(10, pq.Pop());
		assertEquals(12, pq.Pop());
		assertEquals(90, pq.Pop());
		assertEquals(true, pq.Empty());
	});

	test("Update value", []()
	{
		PriorityQueue pq;

		pq.Push(10);
		pq.Push(5);
		pq.Push(8);
		pq.Push(12);
		pq.Push(1);
		pq.Push(90);
		pq.Push(9);

		pq.Update(70, 5);
		pq.Update(7, 12);

		assertEquals(1, pq.Pop());
		assertEquals(7, pq.Pop());
		assertEquals(8, pq.Pop());
		assertEquals(9, pq.Pop());
		assertEquals(10, pq.Pop());
		assertEquals(70, pq.Pop());
		assertEquals(90, pq.Pop());
		assertEquals(true, pq.Empty());
	});

	test("Performance", []()
	{
		constexpr auto size = 1000000;
		std::default_random_engine random_engine(time(nullptr));
		std::uniform_int_distribution<int> distribution(1,9999999);
		auto rand = std::bind ( distribution, random_engine );

		std::vector<int> test;
		PriorityQueue pq;

		for(int i = 0 ; i < size; i++)
			pq.Push(rand());

		while(!pq.Empty())
			test.push_back(pq.Pop());

		assertEquals(true, std::is_sorted(test.begin(), test.end()));
	});

	return 0;
}
