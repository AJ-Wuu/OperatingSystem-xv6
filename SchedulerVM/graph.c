#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

int sleeping_time = 10;

void printinfo(int pid) {
  struct pstat ps = {0};
  getpinfo(&ps);

  for (int i = 0; i < NPROC; i++) {
    if (ps.pid[i] == pid) {
      printf(0, "pid = %d has tickets = %d, ticks = %d, in use = %d\n", pid, ps.tickets[i], ps.ticks[i], ps.inuse[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  int pid1, pid2, pid3;
  if ((pid1 = fork()) == 0) {
    int pp1 = getpid();
    printf(0, "pid %d started\n", pp1);
    settickets(1);
    sleep(sleeping_time);
    printinfo(pp1);
    printf(0, "pid %d finished\n", pp1);
    exit(); 
  }
  
  if ((pid2 = fork()) == 0) {
    int pp2 = getpid();
    printf(0, "pid %d started\n", pp2);
    settickets(0);
    sleep(sleeping_time);
    printinfo(pp2);
    printf(0, "pid %d finished\n", pp2);
    exit();
  }
  
  if ((pid3 = fork()) == 0) {
    int pp3 = getpid();
    printf(0, "pid %d started\n", pp3);
    settickets(1);
    sleep(sleeping_time);
    printinfo(pp3);
    printf(0, "pid %d finished\n", pp3);
    exit();
  }

  wait();
  wait();
  wait();

  exit();
}
