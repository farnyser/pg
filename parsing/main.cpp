#include <cstdlib>
#include "../test/assert.hpp"
#include "parser.hpp"

using namespace pg::parsing;

template <char c>
struct ParseChar
{
	static ParsingResult Parse(const std::string& in) 
	{
		if(in.length() == 0 || in[0] != c)
			return ParsingError{};

		return ParsingSuccess{std::string{} + c, in.substr(1)};
	}
};

template <char s, char e>
struct CharRange
{
	static ParsingResult Parse(const std::string& in) 
	{
		if(in.length() == 0 || (in[0] < s | in[0] > e))
			return ParsingError{};

		return ParsingSuccess{std::string{} + in[0], in.substr(1)};
	}
};


int main()
{
	auto c = Or<ParseChar<'a'>, 
				ParseChar<'b'>, 
				ParseChar<'c'>>{};

	auto cc = And<CharRange<'a', 'z'>, decltype(c)>{};
	auto mx = Many<ParseChar<'x'>>{};

	assertEquals(ParseChar<'x'>::Parse("xab").Success.HasValue(), true);
	assertEquals(ParseChar<'x'>::Parse("ab").Success.HasValue(), false);
	assertEquals(decltype(c)::Parse("ab").Success.HasValue(), true);
	assertEquals(decltype(c)::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bxab").Success.HasValue(), false);
	assertEquals(decltype(cc)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("bab").Success.HasValue(), false);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success->Remaining, "bab");

	return EXIT_SUCCESS;
}
