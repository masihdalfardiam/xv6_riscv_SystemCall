#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
extern int sys_kfreemem(void);

// An array mapping syscall numbers from syscall.h
// to the function that handles the system call.
static uint64 (*syscalls[])(void) = {
        [SYS_fork]    sys_fork,
        [SYS_exit]    sys_exit,
        [SYS_wait]    sys_wait,
        [SYS_pipe]    sys_pipe,
        [SYS_read]    sys_read,
        [SYS_kill]    sys_kill,
        [SYS_exec]    sys_exec,
        [SYS_fstat]   sys_fstat,
        [SYS_chdir]   sys_chdir,
        [SYS_dup]     sys_dup,
        [SYS_getpid]  sys_getpid,
        [SYS_sbrk]    sys_sbrk,
        [SYS_sleep]   sys_sleep,
        [SYS_uptime]  sys_uptime,
        [SYS_open]    sys_open,
        [SYS_write]   sys_write,
        [SYS_mknod]   sys_mknod,
        [SYS_unlink]  sys_unlink,
        [SYS_link]    sys_link,
        [SYS_mkdir]   sys_mkdir,
        [SYS_close]   sys_close,
        [SYS_kfreemem] sys_kfreemem,
};
