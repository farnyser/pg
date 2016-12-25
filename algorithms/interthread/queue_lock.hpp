#ifndef __ADAPTOR_QUEUE_LOCK_HPP__
#define __ADAPTOR_QUEUE_LOCK_HPP__

#include <queue>
#include <thread>
#include <mutex>

namespace pg 
{
	namespace adaptor
	{
		template <typename T>
		class QueueLock
		{
			private:
				std::queue<T> q;
				std::mutex m;
			
			public:
				template <typename F>
				size_t try_consume(F f) {
					std::lock_guard<std::mutex> lock(m);
					
					if(!q.empty())
					{
						f(q.front());
						
						q.pop();
						return true;
					}
					
					return false;
				}
				
				template <typename F>
				size_t try_push(F f) {
					std::lock_guard<std::mutex> lock(m);
					T t;
					f(t);
					q.push(t);
					return true;
				}
		};
	}
}

#endif 