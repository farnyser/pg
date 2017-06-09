#include "../parser.hpp"
#include "../parser.basics.hpp"
#include <map>

using namespace pg::parsing;

using ws = Ignore<ManyOrNone<Or<Char<' '>, Char<'\t'>, Char<'\r'>, Char<'\n'>>>>;
using digit = Many<CharRange<'0', '9'>>;
using decimal = And<Ignore<ws>, 
	           And<Or<
				And<digit, Optional<Char<'.'>>, Optional<digit>>,
				And<Optional<digit>, Optional<Char<'.'>>, digit>
		       >>, Optional<And<Or<Char<'e'>, Char<'E'>>, digit>>,
			   Ignore<ws>>;
using identifier = And<ws, Many<CharRange<'a', 'z'>>, ws>;

struct apply_biop
{
	std::string operator()(const std::vector<std::string>& result)
	{
		auto a = std::atof(result[0].c_str());
		auto b = std::atof(result[2].c_str());
		long r = 0; // todo  ; double ?

		if(result[1] == "+")
			r = a+b;
		if(result[1] == "-")
			r = a-b;
		if(result[1] == "*")
			r = a*b;
		if(result[1] == "/")
			r = a/b;

		return std::to_string(r);
	}
};

static std::map<std::string, std::string> values;

struct find_value
{
	std::string operator()(const std::vector<std::string>& result)
	{
		values["pi"] = "3.14";
		return values[result[0]];
	}
};

struct set_value 
{
	std::string operator()(const std::vector<std::string>& in)
	{
		values[in[0]] = in[2];
		return in[2];
	};
};

using highop = And<ws, Or<Char<'*'>, Char<'/'>>, ws>;
using lowop = And<ws, Or<Char<'+'>, Char<'-'>>, ws>;

struct op_self;
struct tm_self;

using factor = Or<
					And<Ignore<Char<'('>>, Self<op_self>, Ignore<Char<')'>>>, 
					Transform<identifier, find_value>,
					decimal
				>;
using term = Recurse<tm_self, Or<
		Transform<And<factor, highop, Self<tm_self>>, apply_biop>,
		factor	
		>>;

using op = Recurse<op_self, Or<
			Transform<And<term, lowop, Self<op_self>>, apply_biop>,
			term
			>>;

using assignement = Transform<And<identifier, Char<'='>, op>, set_value>;

using expression = Or<assignement, op>;

