#ifndef __SINGLE_CONSUMER_HPP__
#define __SINGLE_CONSUMER_HPP__

namespace pg 
{
	namespace lockfree
	{
		template <typename T, size_t SIZE, size_t PRODUCER_COUNT>
		class SingleConsumer
		{
			private:
				unsigned long rp;
			
			public:
				class Publisher
				{ 
					private:
						std::atomic<unsigned long> read{0}, write{0};
						unsigned long r{0}, w{0};
						std::array<T, SIZE> buffer;
					
					public:
						template <typename F>
						bool try_consume(F f) {
							if(r < write) {
								f(buffer[r%SIZE]);
								r++;
								read++;
								return true;
							}
							return false;
						}
					
						template <typename F>
						bool try_push(F f) {
							if(w - read < SIZE) {
								f(buffer[w%SIZE]);
								write++;
								w++;
								return true;
							}
							return false;
						}
				};
				
				template <typename F>
				bool try_consume(F f) {
					for(size_t i = 0; i < PRODUCER_COUNT; i++) {
						auto&p = Publisher[rp++ % PRODUCER_COUNT];
						if(p.try_consume(f))
							return true;
					}
					return false;
				}				

				std::array<Publisher, PRODUCER_COUNT> Publisher;
		};
	}
}

#endif 