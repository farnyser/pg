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

static std::map<std::string, std::string> functions;

struct call
{
	std::string operator()(const std::vector<std::string>& result);
};

struct define 
{
	std::string operator()(const std::vector<std::string>& in)
	{
		functions[in[0]] = in[1];
		return in[1];
	};
};

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
struct op_list_self;

using op_list = Recurse<op_list_self, 
					Or<
						And<Self<op_self>, ws, Char<','>, ws, Self<op_list_self>>,
						Self<op_self>
					>>;
using fct_call = Transform<
						And<identifier, Ignore<Char<'('>>, Optional<op_list>, Ignore<Char<')'>>>
						, call>;
using factor = Or<
					And<Ignore<Char<'('>>, Self<op_self>, Ignore<Char<')'>>>, 
					fct_call,
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

using fct_def = Transform<
		And<
			identifier, 
			Ignore<And<Char<'('>, ws, Char<')'>, ws, Char<'='>>>, 
			Many<NotChar<'\n'>>
		>, define>; 

using assignement = Transform<And<identifier, Char<'='>, op>, set_value>;

using expression = Or<assignement, fct_def, op>;

	std::string call::operator()(const std::vector<std::string>& result)
	{
		auto r = functions[result[0]];

		for(size_t i = 0; i+1 < result.size(); i++)
		{
			auto key = "{" + std::to_string(i) + "}";
			
			for(auto it = r.find(key); it != std::string::npos; it = r.find(key))
				r.replace(it, key.size(), result[i+1]);
		}

		return op::Parse(r).Success->Flat();
	}


