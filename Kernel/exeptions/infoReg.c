#include <inforeg.h>
#include <naiveConsole.h>

const static char *registers_names[] = {"rax", "rbx", "rcx", "rdx", "rbp", "rdi", "rsi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
static uint64_t registers_values[15] = {0};
static uint64_t rip, rsp;

void saveIp(uint64_t _rip)
{
    rip = _rip;
}

void saveRsp(uint64_t _rsp)
{
    rsp = _rsp;
}

void saveReg(int i, uint64_t reg)
{
    registers_values[14 - i] = reg;
}

void printRegs()
{
    ncPrint("RIP: ");
    ncPrintHex(rip);
    ncPrintChar('\n');

    ncPrint("RSP: ");
    ncPrintHex(rsp);
    ncPrintChar('\n');

    for (int i = 0; i < 15; i++)
    {
        ncPrint(registers_names[i]);
        ncPrintChar(':');
        ncPrintChar(' ');
        ncPrintHex(registers_values[i]);
        ncPrintChar('\n');
    }
}