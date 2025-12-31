#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//2 settickets 함수 구현
int
sys_settickets(void)
{
  int tickets, end_ticks;
  //사용자 인자 0번째 가져오고, 실패시 -1리턴
  if(argint(0, &tickets) < 0) return -1;

  //사용자 인자 1번째 가져오고, 없으면 0
  argint(1, &end_ticks);

  //티켓수가 1미만이거나, STRIDE_MAX 초과하면 에러
  if(tickets < 1 || tickets > STRIDE_MAX)
	  return -1;

  //현재 실행 중인 프로세스 포인터 가져와서 값 세팅
  struct proc *p = myproc();
  p->tickets = tickets;
  p->stride = STRIDE_MAX / tickets;
  if(end_ticks >= 1) p->end_ticks = end_ticks;

  return 0;
}
