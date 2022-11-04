#include <naiveConsole.h>
#include <inforeg.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if (exception == INVALID_OPCODE_ID)
		invalid_opcode();
	printRegs();

}
void zero_division()
{
	ncPrintChar('\n');
	ncPrintFormat("Division By Zero ", RED, BLACK);
	ncPrintChar('\n');
}

void invalid_opcode()
{
	ncPrintChar('\n');
	ncPrintFormat("Invalid OPCODE ", RED, BLACK);
	ncPrintChar('\n');
}