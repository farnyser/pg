#include <iostream>
#include <iomanip>
#include <future>
#include "../../test/assert.hpp"
#include "single_consumer.hpp"
#include "multiple_consumer.hpp"
#include "queue_lock.hpp"

#define EVENT_COUNT 200000

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
void run_test(size_t count, std::initializer_list<R*> readers, std::initializer_list<P*> producers)
{
	auto nb_producer = producers.size();
	auto expected_sum = nb_producer * (count*(count-1))/2;
	
	std::vector<std::future<double>> handlers;
	std::vector<std::future<std::pair<double, unsigned long>>> read_handlers;
	for(auto p : producers) 
		handlers.push_back(std::async(std::launch::async, [=](){ return produce(*p, count); }));
	for(auto r : readers) 
		read_handlers.push_back(std::async(std::launch::async, [=](){ return consume(*r, count*nb_producer); }));
	
	std::cout << std::setprecision(2) << "[99.9th.prctl";
	for(auto& r : read_handlers) {
		auto result = r.get();
		auto rl = result.first;
		assertEquals(expected_sum, result.second);
		std::cout << ", r: " << rl << "us";
	}
	for(auto& p : handlers)
		std::cout << ", p: " << p.get() << "us";
	std::cout << "]";
}

int main()
{	
	std::cout << "=== 1 READER & 1 PRODUCER ===" << std::endl;
	
	test("Queue", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		run_test(EVENT_COUNT, {&buffer}, {&buffer}); 
	});
			
	test("Single Consumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 1> buffer;
		run_test(EVENT_COUNT, {&buffer}, {&buffer.Publisher[0]}); 
	});

	test("Multiple Consumer", [](){ 
		pg::lockfree::MultipleConsumer<Event, 4096, 1, 1> buffer;
		run_test(EVENT_COUNT, {&buffer.Consumer[0]}, {&buffer.Publisher[0]}); 
	});

	std::cout << std::endl;
	std::cout << "=== 1 READER & 2 PRODUCERS ===" << std::endl;
	
	test("Queue", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		run_test(EVENT_COUNT/2, {&buffer}, {&buffer, &buffer}); 
	});
	
	test("Single Consumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 2> buffer;
		run_test(EVENT_COUNT/2, {&buffer}, {&buffer.Publisher[0], &buffer.Publisher[1]}); 
	});
	
	test("Multiple Consumer", [](){ 
		pg::lockfree::MultipleConsumer<Event, 4096, 2, 1> buffer;
		run_test(EVENT_COUNT/2, {&buffer.Consumer[0]}, {&buffer.Publisher[0], &buffer.Publisher[1]}); 
	});

	std::cout << std::endl;
	std::cout << "=== 1 READER & 4 PRODUCERS ===" << std::endl;
	
	test("Queue", [](){ 
		pg::adaptor::QueueLock<Event> buffer;
		run_test(EVENT_COUNT/4, {&buffer}, {&buffer, &buffer, &buffer, &buffer}); 
	});
	
	test("Single Consumer", [](){ 
		pg::lockfree::SingleConsumer<Event, 4096, 4> buffer;
		run_test(EVENT_COUNT/4, {&buffer}, {&buffer.Publisher[0], &buffer.Publisher[1], &buffer.Publisher[2], &buffer.Publisher[3]}); 
	});
	
	test("Multiple Consumer", [](){ 
		pg::lockfree::MultipleConsumer<Event, 4096, 4, 1> buffer;
		run_test(EVENT_COUNT/4, {&buffer.Consumer[0]}, {&buffer.Publisher[0], &buffer.Publisher[1], &buffer.Publisher[2], &buffer.Publisher[3]}); 
	});

	std::cout << std::endl;
	std::cout << "=== 2 READERS & 1 PRODUCER ===" << std::endl;
		
	test("Multiple Consumer", [](){ 
		pg::lockfree::MultipleConsumer<Event, 4096, 1, 2> buffer;
		run_test(EVENT_COUNT, {&buffer.Consumer[0], &buffer.Consumer[1]}, {&buffer.Publisher[0]}); 
	});
	
	std::cout << std::endl;
	std::cout << "=== 2 READERS & 2 PRODUCERS ===" << std::endl;
	
	test("Multiple Consumer", [](){ 
		pg::lockfree::MultipleConsumer<Event, 4096, 2, 2> buffer;
		run_test(EVENT_COUNT/2, {&buffer.Consumer[0], &buffer.Consumer[1]}, {&buffer.Publisher[0], &buffer.Publisher[1]}); 
	});

	return EXIT_SUCCESS;
}