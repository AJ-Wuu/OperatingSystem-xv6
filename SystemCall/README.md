# p1b - AiJing Wu, awu53, 9081879174
## Process
1. Declared functions in usys.S, syscall.h, syscall.c, user.h (I might missed listing some here, these are the files I remembered from the top of my head)
2. Defined functions in sysfile.c
    i. Defined global char *trace_pathname and int trace_count
    ii. Defined sys_trace() to update trace_pathname and trace_count, and if there is no input path, return -1
    iii. Definied sys_getcount() to return current trace_count number
    iv. Updated sys_open() to compare if the openning path is exactly trace_pathname, if it is and opens successfully, then trace_count increased by 1

## Test
I added one personal test (which I commented out) traceiputtest in usertests.c to check if trace() and getcount() work as desired

## Reflection
1. The structure really takes a while to understand
2. It might be better to save a "flag" of start tracing, but I used the global trace_pathname together with it -> not ideal space-wise
