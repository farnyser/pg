#ifndef __PG_PARSER_HPP__
#define __PG_PARSER_HPP__

#include <string>
#include <stdexcept>
#include <set>
#include "parser.core.hpp"

namespace pg
{
	namespace parsing	
	{
		template <typename...ARGS> 
		class Or
		{
			private:
				template <typename T = ErrorParser, typename...OTHERS>
				static ParsingResult InternalParse(const std::string& input)
				{
					auto result = T::Parse(input);
					if (result.Success.HasValue() || std::is_same<T, ErrorParser>::value)
						return result;

					return InternalParse<OTHERS...>(input);
				}

			public: 
				static ParsingResult Parse(const std::string& input) 
				{
					return InternalParse<ARGS...>(input);
				};
		};

		template <typename...ARGS>
		class And
		{
			
			private:
				template <typename T = SuccessParser, typename...OTHERS>
				static ParsingResult InternalParse(const std::string& input)
				{
					auto result = T::Parse(input);
					if (result.Error.HasValue() || std::is_same<T, SuccessParser>::value)
						return result;

					auto remaining = InternalParse<OTHERS...>(result.Success.Value().Remaining);
					if (remaining.Error.HasValue())
						return ParsingError{};

					auto success = ParsingSuccess { 
						result.Success->Content, 
						remaining.Success->Remaining 
					};

					for(auto& x : remaining.Success->Content)
						success.Content.push_back(x);

					return success;
				}

			public: 
				static ParsingResult Parse(const std::string& input) 
				{
					return InternalParse<ARGS...>(input);
				};
		};

		template <typename P>
		class Optional 
		{
			public:
				static ParsingResult Parse(const std::string& input)
				{
					return Or<P, SuccessParser>::Parse(input);
				}
		};

		template <typename P>
		class Many 
		{
			public:
				static ParsingResult Parse(const std::string& input) 
				{
					bool success = false;
					std::string matched, remaining;

					while(true) 
					{
						auto result = P::Parse(success ? remaining : input);
						if (result.Error.HasValue())
							break;
						
						matched += result.Success->Flat();
						remaining = result.Success->Remaining;
						success = true;
					}

					if(success)
						return ParsingSuccess{{matched}, remaining};

					return ParsingError{};
				};
		};

		template <typename P>
		class ManyOrNone
		{
			public:
				static ParsingResult Parse(const std::string& input)
				{
					return Optional<Many<P>>::Parse(input);
				}
		};

		template <typename P>
		struct Ignore
		{
			static ParsingResult Parse(const std::string& input)
			{
				auto result = P::Parse(input);
				if(result.Success.HasValue())
					return ParsingSuccess{{}, result.Success->Remaining};

				return ParsingError{};
			}
		};

		struct IParser
		{
			virtual ParsingResult ParseVirtual(const std::string& input) = 0;
			virtual ~IParser() = default;
		};
		
		template <typename ID>
		struct Self 
		{
			static IParser* self(IParser* parser = nullptr) 
			{ 
				static IParser* p = parser;
				return p; 
			};

			static ParsingResult Parse(const std::string& input)
			{
				return self()->ParseVirtual(input);
			}
		};

		template <typename ID, typename P>
		struct Recurse : IParser
		{
			virtual ParsingResult ParseVirtual(const std::string& input)
			{
				return P::Parse(input);
			}

			static ParsingResult Parse(const std::string& input)
			{
				Self<ID>::self(new Recurse<ID, P>{});
				return P::Parse(input);
			}
		};
		
		template <typename P, typename X>
		struct Transform
		{
			static ParsingResult Parse(const std::string& input)
			{
				auto result = P::Parse(input);
				if(result.Success.HasValue())
					return ParsingSuccess{{X{}(result.Success->Content)}, result.Success->Remaining};
				return result;
			}
		};
		
	};
} 

#endif /* end of include guard: __PG_PARSER_HPP__ */
