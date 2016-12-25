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
void test_single_consumer(size_t count, R& reader, std::initializer_list<P*> producers)
{
	auto nb_producer = producers.size();
	auto expected_sum = nb_producer * (count*(count-1))/2;
	
	auto consumer = std::async(std::launch::async, [&](){ return consume(reader, count*nb_producer); });
	
	std::vector<std::future<double>> handlers;
	for(auto& p : producers) 
		handlers.push_back(std::async(std::launch::async, [&](){ return produce(*p, count); }));
	
	auto result = consumer.get();
	auto rl = result.first;
	
	assertEquals(expected_sum, result.second);
	
	std::cout << std::setprecision(2) << "[99.9th.prctl r: " << rl << "us";
	for(auto& p : handlers)
		std::cout << ", p: " << p.get() << "us";
	std::cout << "]";
}

int main()
{	
	std::cout << "=== 1 READER & 2 PRODUCERS ===" << std::endl;
	
	test("Queue", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		test_single_consumer(100000, buffer, {&buffer, &buffer}); 
	});
	
	test("Single Consumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 3> buffer;
		test_single_consumer(100000, buffer, {&buffer.Publisher[0], &buffer.Publisher[1]}); 
	});

	std::cout << std::endl;
	std::cout << "=== 1 READER & 3 PRODUCERS ===" << std::endl;
	
	test("Queue", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		test_single_consumer(100000, buffer, {&buffer, &buffer, &buffer}); 
	});
	
	test("Single Consumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 3> buffer;
		test_single_consumer(100000, buffer, {&buffer.Publisher[0], &buffer.Publisher[1], &buffer.Publisher[2]}); 
	});

	return EXIT_SUCCESS;
}