#ifndef __MULTIPLE_CONSUMER_HPP__
#define __MULTIPLE_CONSUMER_HPP__

namespace pg 
{
	namespace lockfree
	{
		template <typename T, size_t SIZE, size_t PRODUCER_COUNT, size_t CONSUMER_COUNT>
		class MultipleConsumer
		{			
			public:			
				class Publisher
				{ 
					private:
						friend class MultipleConsumer;
						
						MultipleConsumer* self;	
						size_t id;
						std::atomic<unsigned long> write;
						unsigned long w;
						std::array<T, SIZE> buffer;
						
						void init(MultipleConsumer* s, size_t i) 
						{
							self = s;
							write = 0;
							w = 0;
							id = i;
						}
						
					public:
					
						unsigned long min_read() {
							unsigned long min = -1;
							for(auto& r : self->Consumer) 
							{
								unsigned long re = r.reads[id];
								if(re < min) 
									min = re;
							}
							return min;
						}
					
						template <typename F>
						bool try_consume(F f, unsigned long r) {
							if(r < write) {
								f(buffer[r%SIZE]);
								return true;
							}
							return false;
						}
					
						template <typename F>
						bool try_push(F f) {							
							if(w - min_read() < SIZE) {
								f(buffer[w%SIZE]);
								write++;
								w++;
								return true;
							}
							return false;
						}
				};
				
				class Consumer
				{ 
					private:
						friend class MultipleConsumer;
						friend class Publisher;
					
						MultipleConsumer* self;	
						std::array<std::atomic<unsigned long>, PRODUCER_COUNT> reads;
						unsigned long rp;
					
						void init(MultipleConsumer* s) 
						{
							self = s;
							for(size_t i = 0; i < PRODUCER_COUNT; i++) 
								reads[i] = 0;
							rp = 0;
						}

					public:		

						template <typename F>
						bool try_consume(F f) {
							for(size_t i = 0; i < PRODUCER_COUNT; i++) {
								auto x = rp++ % PRODUCER_COUNT;
								auto& p = self->Publisher[x];
								unsigned long r = reads[x];
								if(p.try_consume(f, r)) {
									reads[x]++;
									return true;
								}
							}
							return false;
						}				
				};

				std::array<Publisher, PRODUCER_COUNT> Publisher;
				std::array<Consumer, CONSUMER_COUNT> Consumer;

				MultipleConsumer()
				{					
					size_t id = 0;
					for(auto&p : Publisher)
						p.init(this, id++);					
					for(auto&c : Consumer)
						c.init(this);
				}
		};
	}
}

#endif 