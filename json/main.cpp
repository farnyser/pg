#include <iostream>
#include <type_traits>
#include <cstdlib>

#include "json.hpp"

/*
template <typename T> 
void unserialize(const Serializable<const char*, T>& data)
{
	*data.value = 0;
}
*/

class Test : public pg::json::ISerializable<Test>
{
public:
	int x, y;

	SERIALIZABLE(_(x), _(y))
};


class Person : public pg::json::ISerializable<Person>
{
	int age;
	std::string name;
	std::string toto;
	std::vector<int> array;
	std::vector<Test> sar;
	std::set<std::string> set;

	Test t;

public:
	Person(int age, const std::string& n) : age(age), name(n) 
	{
		toto = "hey ho \"ah\"";
		t.x = 12;
		t.y = 24;
		array.push_back(123);
		array.push_back(35);
		array.push_back(1009);
		sar.push_back(t);
		sar.push_back(t);
		set.insert("test \"aa\"");
	}

	SERIALIZABLE
	(
		_(age), 
		_(name), 
		_(toto), 
		_(array), 
		_(sar), 
		_(set), 
		$("test", t) 
	)
};


int main(int argc, char **argv)
{
	Test t;
	t.x = 1;
	t.y = 2;
	pg::json::serialize(std::cout, t);
	std::cout << std::endl;

	Person p(42, "jean");
	pg::json::serialize(std::cout, p);
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

