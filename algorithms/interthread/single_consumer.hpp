#ifndef __SINGLE_CONSULER_HPP__
#define __SINGLE_CONSULER_HPP__

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
						std::atomic<unsigned long> read, write;
						unsigned long r, w;
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
					while(true) {
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