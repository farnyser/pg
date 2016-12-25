#include <iostream>
#include <iomanip>
#include <future>
#include "../../test/assert.hpp"
#include "single_consumer.hpp"
#include "queue_lock.hpp"

struct Event 
{
	int id;
	float payload[64];
};

template <typename T>
auto consume(T& buffer, size_t count)
{
	unsigned long sum_id = 0;
	
	auto l = latency([&](){
		while(!buffer.try_consume([&](Event& e) {
			sum_id += e.id;
		}));
	}, count);
		
	return std::make_pair(l, sum_id);
}

template <typename T>
auto produce(T& buffer, size_t count)
{
	unsigned long i = 0;
	
	auto l = latency([&](){
		while(!buffer.try_push([&](Event& e) {
			e.id = i;
			i++;
		}));
	}, count);
	
	return l;
}

template <typename R, typename P>
void test(R& reader, P& p1, P& p2, size_t count)
{
	constexpr size_t nb_producer = 2;
	auto expected_sum = nb_producer * (count*(count-1))/2;
	
	auto consumer = std::async(std::launch::async, [&](){ return consume(reader, count*nb_producer); });
	auto producer1 = std::async(std::launch::async, [&](){ return produce(p1, count); });
	auto producer2 = std::async(std::launch::async, [&](){ return produce(p2, count); });
	
	auto result = consumer.get();
	auto rl = result.first;
	auto p1l = producer1.get();
	auto p2l = producer2.get();
	
	assertEquals(expected_sum, result.second);
	
	std::cout << std::setprecision(2) << "[99.9th.prctl r: " << rl << "us, p1: " << p1l << "us, p2: " << p2l << "us] ";
}

int main()
{	
	test("Queue (locking)", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		test(buffer, buffer, buffer, 100000); 
	});
	
	test("SingleConsumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 2> buffer;
		test(buffer, buffer.Publisher[0], buffer.Publisher[1], 100000); 
	});
	
	return EXIT_SUCCESS;
}