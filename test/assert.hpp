#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <queue>

template <typename T1, typename T2>
void assertEquals(T1 t1, T2 t2)
{
	if(!(t1 == t2))
	{
		std::stringstream ss;
		ss << "Assertion failed: ";
		ss << "Expected:" << t1;
		ss << ", Got:" << t2;
		throw std::runtime_error(ss.str());
	}
}

template <typename T, typename F>
auto time(F f)
{
	auto start = std::chrono::steady_clock::now();
	
	f();
	
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	return std::chrono::duration<double, T>(diff).count();
}



template <typename F>
void test(const std::string& name, F f, double showDuration = 5, std::ostream& os = std::cout)
{
	os << "Running test '" << name << "' ";
	os.flush();
	
	try
	{
		long duration = time<std::milli>(f);
		
		if(duration > showDuration) {
			std::cout << "[" << duration << " ms] ";
		}
		
		os << "-> Success" << std::endl;
	}
	catch(const std::exception& e)
	{
		os << "-> Failed !" << std::endl;
		os << "\t => " << e.what() << std::endl;
	}	
}

template <typename F, typename T = std::micro> 
auto latency(F f, unsigned long count, std::ostream& os = std::cout)
{
	// 99.9th percentile
	auto limit = count / 100;
	
	struct node { unsigned long id; double time; };
	auto cmp = [](const node& a, const node& b) { return b.time < a.time; };
	std::priority_queue<node, std::vector<node>, decltype(cmp)> times(cmp);
	
	for(unsigned long i = 0; i < count ; i++)
	{
		auto duration = time<T>(f);
		
		if(times.size() < limit || duration > times.top().time)
			times.push({i,duration});
		
		if(times.size() > limit)
			times.pop();
	}
	
	return times.top().time;
}