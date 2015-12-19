#include <iostream>
#include <cstdlib>
#include "ddh.hpp"

using namespace pg;

class Foo : 
	public DynamicDataHolder<Foo>, 
	public DynamicDataType<Foo,double>,
	public DynamicDataType<Foo,std::string>
{
public:
	int id;

	void Clear()
	{
		Get<double>().clear();
		Get<std::string>().clear();
	}
};

class Bar : public DynamicDataHolder<Bar>, public DynamicDataType<Bar, double>
{

};

void foo_test()
{
	const int cQuantity = Foo::Register<double>("quantity");
	const int cAmount = Foo::Register<double>("amount");
	const int cName = Foo::Register<std::string>("name");

	Foo f1, f2;

	std::cout << "F1" << std::endl;
	std::cout << "Quantity: " << f1.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f1.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f1.Get<std::string>()[cName]<< std::endl;

	std::cout << "F2" << std::endl;
	std::cout << "Quantity: " << f2.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f2.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f2.Get<std::string>()[cName]<< std::endl;

	auto& f1_doubles = f1.Get<double>();

	f1_doubles.at(cQuantity) = 100;
	f1_doubles.at(cAmount) = 45*f1_doubles.at(cQuantity);
	f1.Get<std::string>()[cName] = "FooBar1";

	f2.Get<std::string>()[cName] = "FooBar2";

	std::cout << "F1" << std::endl;
	std::cout << "Quantity: " << f1.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f1.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f1.Get<std::string>()[cName]<< std::endl;

	std::cout << "F2" << std::endl;
	std::cout << "Quantity: " << f2.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f2.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f2.Get<std::string>()[cName]<< std::endl;

	f1.Clear();

	std::cout << "F1" << std::endl;
	std::cout << "Quantity: " << f1.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f1.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f1.Get<std::string>()[cName]<< std::endl;

	std::cout << "F2" << std::endl;
	std::cout << "Quantity: " << f2.Get<double>()[cQuantity] << std::endl;
	std::cout << "Amount: "   << f2.Get<double>()[cAmount]   << std::endl;
	std::cout << "Name: "     << f2.Get<std::string>()[cName]<< std::endl;
}

void bar_test()
{	
	const int cBaba = Bar::Register<double>("baba");

	Bar b1;

	std::cout << "Baba: "     << b1.Get<double>()[cBaba]<< std::endl;
	b1.Get<double>()[cBaba]++;
	std::cout << "Baba: "     << b1.Get<double>()[cBaba]<< std::endl;
	b1.Get<double>()[cBaba]++;
	std::cout << "Baba: "     << b1.Get<double>()[cBaba]<< std::endl;
	b1.Get<double>()[cBaba]++;
	std::cout << "Baba: "     << b1.Get<double>()[cBaba]<< std::endl;

	b1.Get<double>("overload")[cBaba]++;
	std::cout << "Baba: "     << b1.Get<double>("overload")[cBaba]<< std::endl;

	b1.Get<double>()[cBaba]++;
	std::cout << "Baba: "     << b1.Get<double>()[cBaba]<< std::endl;
}

int main(int argc, char **argv)
{
	bar_test();	
	foo_test();

	return EXIT_SUCCESS;
}