# Summary

## Terminology used in book:
- Task Switching: Multicore behavior in software on one hardware core.
- Hardward concurrency: The processes on actual different internal cores.
- Context switch: When a core is shared between processes, it needs to do a context switch when switching between the differen processes.

## Concurrency with multiple processes
- use IPC: Inter Process Communication protocol to send messages.
- safe 
- slow

## Concurrency with Threads
- Shared memory space
- unsafe (handle the concurrent access yourself)
- fast

## Parallel vs Concurrent
- Parallel: used when talking about performance
- Concurrency: used when talking about seperation of concerns or responsivenss.
