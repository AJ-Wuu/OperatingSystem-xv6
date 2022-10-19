# xv6-systemcall
## Part 1: xv6 on QEMU
1. ```tar xvzf ~cs537-1/public/xv6.tgz```
2. Run ```make``` to create xv6.img
3. Run ```make qemu-nox``` to run xv6 on linprog
4. Run 'Ctrl-A X' to quit

## Part 2: The life of getpid
### Check all files: ```make clean | grep "getpid" *```
### User Space
#### getpid()
- getpid() is executed from a user program in the userspace
```c
void
getpid_test(void)
{
  int ppid;

  ppid = getpid();                          // usertests.c executes getpid() in the userspace
  printf(1, "getpid_test = %d\n", ppid);
}
```
- System calls are declared in user.h: ```int getpid(void);```
#### sys_getpid()
- sys_getpid() is declared in syscall.c: 
```
extern int sys_getpid(void);
...
[SYS_getpid]  sys_getpid,
```
- sys_getpid() is defined in sysproc.c (some other functions are in sysfile.c):
```
int
sys_getpid(void)
{
  return myproc()->pid;
}
```
#### Connecting getpid() and sys_getpid()
- System call number is defined in syscall.h: ```#define SYS_getpid 11```
- SYSCALL() is defined in usys.S and SYSCALL(getpid) is declared in usys.S:
```c
#define SYSCALL(name) \
  .globl name; \                  # declare command name as a global symbol
  name: \                         # entry point of each command
    movl $SYS_ ## name, %eax; \   # put system call number in eax register
    int $T_SYSCALL; \             # trigger software interrupt, enter kernel
    ret                           # return result to the caller of close		
```
- $T_SYSCALL is defined in traps.h

### Kernel
-	int $T_SYSCALL; 	#triggers software interrupt
-	CPU pauses, and asks the interrupt handler to take over
-	Interrupt handler is vector of 64 found in traps.h
```c
#define T_SYSCALL       64      // system call
```
-	vectors.S sends all jumps to alltraps function in trapasm.S
-	alltraps creates the trapframe and calls
-	trap(struct trapframe *tf) in trap.c
-	struct trapframe in x86.h saves the userspace registers and the tf->eax contains the system call number for SYS_getpid
```c
//PAGEBREAK: 36
// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe {
// registers as pushed by pusha
uint edi;
uint esi;
.
.
.
```
Syscall Dispatch
-	trap(struct trapframe *tf) in trap.c calls syscall(void) because tf->trapno == T_SYSCALL
-	trapframe is saved to the process control block
-	After trapframe returns, trap() returns to alltraps
-	Restores the user registers
-	Returns back to the user space with iret
-	sysproc() reads the sysproc number in eax, and calls sys_getpid
-	sys_getpid is defined in sysproc.c
-	Return value saved in proc->pid;
-	Control is returned to trap
-	sys_getpid in sysproc.c returns the integer value of the pid
