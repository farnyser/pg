#include <cstdlib>
#include "../test/assert.hpp"
#include "parser.hpp"
#include "parser.basics.hpp"

using namespace pg::parsing;

using ws = ManyOrNone<Or<Char<' '>, Char<'\t'>, Char<'\r'>, Char<'\n'>>>;
using digit = Many<CharRange<'0', '9'>>;
using decimal = And<Ignore<ws>, 
	           And<Or<
				And<digit, Optional<Char<'.'>>, Optional<digit>>,
				And<Optional<digit>, Optional<Char<'.'>>, digit>
		       >>, Optional<And<Or<Char<'e'>, Char<'E'>>, digit>>>;
				
using integer = And<Ignore<ws>, digit, Ignore<ws>>;

using quoted = 	And<
					Ignore<Char<'"'>>,
					ManyOrNone<Or<And<Ignore<Char<'\\'>>, Char<'"'>>, NotChar<'"'>>>, 
					Ignore<Char<'"'>>
				>;
				

struct block_self;
using block = Recurse<block_self, And<
				Char<'{'>, 
				ManyOrNone<
					Or<
						Self<block_self>,
						NotChar<'}'>>
					>, 
				Char<'}'>
			   >>;

using c = Or<Char<'a'>, 
				Char<'b'>, 
				Char<'c'>>;

using add = And<integer, Char<'+'>, integer>;

int main()
{
	auto cc = And<CharRange<'a', 'z'>, c>{};
	auto mx = Many<Char<'x'>>{};

	auto id = Many<And<Many<CharRange<'a', 'z'>>,
					   Optional<Char<'-'>>>>{};

	assertEquals(Char<'x'>::Parse("xab").Success.HasValue(), true);
	assertEquals(Char<'x'>::Parse("ab").Success.HasValue(), false);
	assertEquals(c::Parse("ab").Success.HasValue(), true);
	assertEquals(c::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("bxab").Success.HasValue(), false);
	assertEquals(decltype(cc)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(cc)::Parse("xbab").Success->Content.size(), 2);
	assertEquals(decltype(cc)::Parse("xbab").Success->Content[0], "x");
	assertEquals(decltype(cc)::Parse("xbab").Success->Content[1], "b");
	assertEquals(decltype(mx)::Parse("xbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("bab").Success.HasValue(), false);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success.HasValue(), true);
	assertEquals(decltype(mx)::Parse("xxxxbab").Success->Remaining, "bab");
	assertEquals(decltype(id)::Parse("abc-hel+").Success.HasValue(), true);
	assertEquals(decltype(id)::Parse("abc-hel+").Success->Remaining, "+");

	assertEquals(ws::Parse("   hello").Success.HasValue(), true);
	assertEquals(ws::Parse("   hello").Success->Remaining, "hello");

	assertEquals(integer::Parse("		912.33").Success->Flat(), "912");
	assertEquals(decimal::Parse("		912.33").Success->Flat(), "912.33");
	assertEquals(decimal::Parse("		912.33e12").Success->Flat(), "912.33e12");
	
	assertEquals(quoted::Parse("\"\"").Success->Flat(), "");
	assertEquals(quoted::Parse("\"hello\"").Success->Flat(), "hello");
	assertEquals(quoted::Parse("\"hi \\\"world\\\"\"").Success->Flat(), "hi \"world\"");
	
	assertEquals(block::Parse("{ return 1 + 2; }").Success->Flat(), "{ return 1 + 2; }");
	assertEquals(block::Parse("{ if(){}; }").Success->Flat(), "{ if(){}; }");

	assertEquals(add::Parse("123 + 456").Success->Content[0], "123");
	assertEquals(add::Parse("123 + 456").Success->Content[1], "+");
	assertEquals(add::Parse("123 + 456").Success->Content[2], "456");

	return EXIT_SUCCESS;
}
