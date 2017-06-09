#include <cstdlib>
#include <iostream>
#include "calc.parser.hpp"
#include "linenoise/linenoise.h"

int main()
{
	char * line = nullptr;
    while((line = linenoise("> ")) != NULL)
	{
		std::string input = line;
		linenoiseHistoryAdd(line); 
		free(line);

		auto result = expression::Parse(input);
		if(result.Success.HasValue() && result.Success->Remaining.length() == 0)
			std::cout << result.Success->Flat() << std::endl;
		else 
			std::cout << "??" << std::endl;
	}

	return EXIT_SUCCESS;
}
