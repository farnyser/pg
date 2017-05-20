#ifndef __PG_PARSER_HPP__
#define __PG_PARSER_HPP__

#include <string>
#include <stdexcept>
#include "optional.hpp"

namespace pg
{
	namespace parsing
	{
		struct ParsingSuccess 
		{
			std::string Content;
			std::string Remaining;
		};

		struct ParsingError {};
		struct ParsingResult 
		{
			utils::Optional<ParsingSuccess> Success;
			utils::Optional<ParsingError> Error;

			ParsingResult(ParsingSuccess&& s) : Success(std::move(s)) {}
			ParsingResult(ParsingError&& e) : Error(std::move(e)) {}
		};

		struct ErrorParser 
		{ 
			static ParsingResult Parse(const std::string&) 
			{ 
				return ParsingError{}; 
			} 
		}; 
		
		struct SuccessParser 
		{ 
			static ParsingResult Parse(const std::string& input) 
			{ 
				return ParsingSuccess{"", input}; 
			} 
		}; 

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

					return ParsingSuccess { 
						result.Success->Content + remaining.Success->Content, 
						remaining.Success->Remaining 
					};
				}

			public: 
				static ParsingResult Parse(const std::string& input) 
				{
					return InternalParse<ARGS...>(input);
				};
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
						
						matched += result.Success->Content;
						remaining = result.Success->Remaining;
						success = true;
					}

					if(success)
						return ParsingSuccess{matched, remaining};

					return ParsingError{};
				};
		};
	}
} 

#endif /* end of include guard: __PG_PARSER_HPP__ */
