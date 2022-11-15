#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "mmu.h"
#include "param.h"
#include <stddef.h>

// global values
void* addresses[2 * NPROC];
int pids[2 * NPROC];
int in_use[2 * NPROC]; // in_use = 1; not_in_use = 0

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int
thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  // if we want to make the one-page-size stack be page-aligned, we need to alloc 2 pages
  void *user_stack;
  user_stack = malloc(PGSIZE * 2);
  
  // check if malloc() succeeded
  if (user_stack == 0) {
    return -1;
  }

  // align the stack if needed
  if (((uint)user_stack % PGSIZE) != 0) {
    user_stack += PGSIZE - ((uint)user_stack % PGSIZE);
  }

  int result = clone(start_routine, arg1, arg2, user_stack);
  if (result != -1) {
    for (int i = 0; i < 2 * NPROC; i++) {
      if (in_use[i] == 0) {
	in_use[i] = 1;
        addresses[i] = user_stack;
	pids[i] = result;
	break;
      }
    }
  }
  else {
    free(user_stack);
  }

  return result;
}

int
thread_join()
{
  void *stack;
  int result = join(&stack);

  if (result != -1) {
    for (int i = 0; i < 2 * NPROC; i++) {
      if (result == pids[i] && in_use[i] == 1) {
	in_use[i] = 0;
        pids[i] = -1;
	free(addresses[i]);
	addresses[i] = NULL;
	break;
      }
    }
  }

  return result;
}

// reference: https://xiayingp.gitbook.io/build_a_os/lock/locking-in-xv6
static inline int fetch_and_add(int* variable, int value) {
    __asm__ volatile("lock; xaddl %0, %1"
      : "+r" (value), "+m" (*variable) // input + output
      : // No input-only
      : "memory"
    );
    return value;
}

void
lock_acquire(lock_t *lock)
{
  // always use value = 1
  while (fetch_and_add(&lock->flag, 1) != 0) {
    ; // spinning
  }
}

void
lock_release(lock_t *lock)
{
  lock->flag = 0;
}

void
lock_init(lock_t *lock)
{
  lock->flag = 0;
}
