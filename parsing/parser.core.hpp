#ifndef __PG_PARSER_CORE_HPP__
#define __PG_PARSER_CORE_HPP__

#include <string>
#include <vector>
#include "optional.hpp"

namespace pg
{
	namespace parsing
	{
		struct ParsingSuccess 
		{
			std::vector<std::string> Content;
			std::string Remaining;

			std::string Flat() const
			{ 
				std::string result;
				for(auto&x : Content) 
					result += x;
				return result;
			}
		};

		struct ParsingError 
		{
		};
		
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
				return ParsingSuccess{{}, input}; 
			} 
		};
	};
} 

#endif /* end of include guard: __PG_PARSER_CORE_HPP__ */
