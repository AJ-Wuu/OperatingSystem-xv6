# Adding Threads to xv6
## clone() and join()
1. implement `int clone(void(*fcn)(void *, void *), void *arg1, void *arg2, void *stack)` and `int join(void **stack)` in proc.c
2. add `sys_clone` and `sys_join` in sysproc.c
3. modify other necessary places, like syscall.c, syscall.h, user.h, defs.h, usys.S

## thread_create(), thread_join(), lock_acquire(), lock_release() and lock_init()
1. implement all of them in ulib.c
2. add all their prototypes to user.h

## Note
1. when doing thread_join(), it's important to free the previous pointer
2. need to manually add type lock_t in types.h 
3. to keep track of threads, we need to add a new attribute num_threads in proc.h
4. for spin lock, we keep using value = 1 as default
