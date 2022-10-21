#include <sysCalls.h>

typedef uint64_t (*SysCall) (uint64_t, uint64_t, uint64_t);

// static SysCall sysCalls[10] = { (SysCall)&sys_read, (SysCall)&sys_write, (SysCall)&getRTC};
static SysCall sysCalls[10] = { 0 };

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax) {
    SysCall sysCall = sysCalls[rax];
    if (sysCall != 0) {
        return sysCall(rdi, rsi, rdx);
    }
    return 0;
}