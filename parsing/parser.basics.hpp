#ifndef __PG_PARSER_BASICS_HPP__
#define __PG_PARSER_BASICS_HPP__

#include <string>
#include <stdexcept>

#include "parser.hpp"

namespace pg
{
	namespace parsing
	{
		template <char c>
		struct Char
		{
			static ParsingResult Parse(const std::string& in) 
			{
				if(in.length() == 0 || in[0] != c)
					return ParsingError{};

				return ParsingSuccess{{std::string{} + c}, in.substr(1)};
			}
		};

		template <char s, char e>
		struct CharRange
		{
			static ParsingResult Parse(const std::string& in) 
			{
				if(in.length() == 0 || (in[0] < s | in[0] > e))
					return ParsingError{};

				return ParsingSuccess{{std::string{} + in[0]}, in.substr(1)};
			}
		};

		template <char c>
		struct NotChar
		{
			static ParsingResult Parse(const std::string& in) 
			{
				if(in.length() == 0 || in[0] == c)
					return ParsingError{};
				
				return ParsingSuccess{{std::string{} + in[0]}, in.substr(1)};
			}
		};
	};
} 

#endif /* end of include guard: __PG_PARSER_BASICS_HPP__ */
