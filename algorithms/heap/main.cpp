#include <iostream>
#include "binary_heap.hpp"

#include "../../test/assert.hpp"

int main(int argc, char **argv)
{
	test("basic Push/Pop", []()
	{
		pg::heap::Binary<int> pq;

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
		pg::heap::Binary<int> pq;

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

	return 0;
}
