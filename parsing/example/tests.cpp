#include <cstdlib>
#include "calc.parser.hpp"
#include "../../test/assert.hpp"

int main()
{
	assertEquals(true, op::Parse("25 + 12").Success.HasValue());	
	assertEquals(true, op::Parse("25 + 12 + 14").Success.HasValue());	
	assertEquals("37", op::Parse("25 + 12").Success->Flat());	
	assertEquals("38", op::Parse("25 + 13").Success->Flat());	
	assertEquals("10", op::Parse("25 - 15").Success->Flat());	
	assertEquals("250", op::Parse("25 * 10").Success->Flat());

	assertEquals(std::to_string(25 - 10 * 2), op::Parse("25 - 10 * 2").Success->Flat());
	assertEquals(std::to_string(250 - 2), op::Parse("250- 2").Success->Flat());
	assertEquals(std::to_string(25 * 10 - 2), op::Parse("25 * 10 - 2").Success->Flat());
	assertEquals(std::to_string((25 * 10) - 2), op::Parse("(25 * 10) - 2").Success->Flat());
	assertEquals(std::to_string(25 * (10 - 2)), op::Parse("25 * (10 - 2)").Success->Flat());
	assertEquals(std::to_string(3 * 2 + 25 * (10 - 2) + 7), op::Parse("3 * 2 + 25 * (10 - 2) + 7").Success->Flat());

	return EXIT_SUCCESS;
}
