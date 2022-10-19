# xv6 - Scheduling
## Round Robin (in `proc.c`)
```
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu(); // per-CPU process scheduler
  c->proc = 0; // each CPU calls scheduler() after setting itself up
  
  // round robin
  for (;;) { // scheduler never returns
    // enable interrupts on this processor
    sti();

    // loop over process table looking for process to run
    acquire(&ptable.lock);
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
      if (p->state != RUNNABLE) {
        continue;
      }
      c->proc = p; // found a process to run
      switchuvm(p); // go to user mode and start running that process
      p->state = RUNNING;
      p->tick += 1;
      swtch(&(c->scheduler), p->context); // execute
      switchkvm(); // back to kernel mode and handle the control to scheduler

      // process is done running for now, so it should change its p->state before running the next round
      c->proc = 0;
    }

    release(&ptable.lock);
}
```
