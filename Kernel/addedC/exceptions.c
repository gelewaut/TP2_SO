
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if (exception == INVALID_OPCODE_ID)
		invalid_opcode();
}

static void zero_division() {
	// Handler para manejar excepcíon
}

static void invalid_opcode() {
}