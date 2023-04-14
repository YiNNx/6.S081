#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
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
  if (growproc(n) < 0)
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
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


int
sys_pgaccess(void)
{
  uint64 buf;
  argaddr(0, &buf);
  int len;
  argint(1, &len);
  uint64 bm_address;
  argaddr(2, &bm_address);

  uint64 bitmask = 0;

  if (len > 64) panic("sys_pgaccess - len <= 64");
l
  struct proc* p = myproc();

  for (int i = 0;i < len;i++) {
    pte_t* pte = walk(p->pagetable, buf + i * PGSIZE, 0);
    bitmask = bitmask | ((((*pte) & PTE_A) >> 6) << i);
    *pte = (*pte) & (~PTE_A);
  }

  copyout(p->pagetable, bm_address, (char*)&bitmask, sizeof(bitmask));

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
