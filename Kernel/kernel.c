#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <added.h>
#include <memoryManager.h>
#include <scheduler.h>

#define HEAP_MEMORY_SIZE 128*1024*1024

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const sampleHeapAddress = (void*)0x600000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

/*
static void processTwo () {
    for (int i = 0; i<1; i++){
		ncPrintChar('a');
		halt(1);
		// blockProcess(2);
	}
	killProcess(2);
}

static void processThree () {
	for (int i=0; i<1; i++) {
        ncPrintChar('b');
		halt(1);
		// blockProcess(3);
	}
	killProcess(3);
}

static void processFour () {
	while (1) {
        ncPrintChar('c');
		halt(1);
		// blockProcess(4);
		// unblockProcess(3);
		// unblockProcess(2);
	}
}
*/

int main()
{	
	mem_init((char *)sampleHeapAddress, HEAP_MEMORY_SIZE);
	initScheduler();
	
	addProcess(sampleCodeModuleAddress, 0, NULL, 1, 1);
	
	load_idt();
	/*
	addProcess(&processTwo, 0, NULL, 1);
    addProcess(&processThree, 0, NULL, 10);
    addProcess(&processFour, 0, NULL, 10);

	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	*/

	
	// Fin de codigo de inicial
	// halt(3);
	// ncClear();
	// halt(1);
	
	while(1){
		ncPrintChar('/');
	}

	// uint64_t key;
	// while((key = getKey())) {
		// uint64_t key = getKey();
	    // ncPrintDec(key);
	    // ncPrintChar(' ');
		// ncPrintChar('a');
	// }

	return 0;
}
