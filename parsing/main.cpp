#include <cstdlib>
#include "../test/assert.hpp"
#include "parser.hpp"

using namespace pg::parsing;

template <char c>
struct Char
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

using ws = ManyOrNone<Or<Char<' '>, Char<'\t'>, Char<'\r'>, Char<'\n'>>>;
using digit = Many<CharRange<'0', '9'>>;
using decimal = And<Ignore<ws>, 
	           And<Or<
				And<digit, Optional<Char<'.'>>, Optional<digit>>,
				And<Optional<digit>, Optional<Char<'.'>>, digit>
		       >>, Optional<And<Or<Char<'e'>, Char<'E'>>, digit>>>;
				
using integer = And<Ignore<ws>, digit>;

int main()
{
	auto c = Or<Char<'a'>, 
				Char<'b'>, 
				Char<'c'>>{};

	auto cc = And<CharRange<'a', 'z'>, decltype(c)>{};
	auto mx = Many<Char<'x'>>{};

	auto id = Many<And<Many<CharRange<'a', 'z'>>,
					   Optional<Char<'-'>>>>{};

	assertEquals(Char<'x'>::Parse("xab").Success.HasValue(), true);
	assertEquals(Char<'x'>::Parse("ab").Success.HasValue(), false);
	assertEquals(decltype(c)::Parse("ab").Success.HasValue(), true);
	assertEquals(decltype(c)::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bxab").Success.HasValue(), false);
	assertEquals(decltype(cc)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("bab").Success.HasValue(), false);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success->Remaining, "bab");
	assertEquals(decltype(id)::Parse("abc-hel+").Success.HasValue(), true);
	assertEquals(decltype(id)::Parse("abc-hel+").Success->Remaining, "+");

	assertEquals(ws::Parse("   hello").Success.HasValue(), true);
	assertEquals(ws::Parse("   hello").Success->Remaining, "hello");

	assertEquals(integer::Parse("		912.33").Success->Content, "912");
	assertEquals(decimal::Parse("		912.33").Success->Content, "912.33");
	assertEquals(decimal::Parse("		912.33e12").Success->Content, "912.33e12");

	return EXIT_SUCCESS;
}
