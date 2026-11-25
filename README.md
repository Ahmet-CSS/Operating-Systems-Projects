# Operating Systems Projects

A collection of systems programming projects written in C, exploring concurrent programming, network communication, synchronization primitives, and operating system kernel development.

## Projects

### 1. S-Chat — Multithreaded UDP Chat

A terminal-based peer-to-peer messaging application using UDP sockets and UBC pthreads.

- Five-thread architecture
- Blocking Send/Receive/Reply IPC
- Non-blocking keyboard and network I/O
- Incoming and outgoing message queues
- Message timestamps with microsecond precision
- Architecture-specific build configuration

### 2. Monitor Library

A synchronization library implemented using RT Threads semaphores.

- Mutual exclusion
- Condition variables
- Monitor enter, leave, wait, and signal operations
- Readers-writers synchronization problem

### 3. xv6 Kernel Extensions

Extensions to the xv6 operating system focused on concurrency and synchronization.

- User-level threading
- Mutex synchronization primitives
- Producer-consumer synchronization
- Kernel-level programming

### 4. Custom List Library

A reusable list library written in C, used for message buffering and synchronization operations.

## Technologies

- C (GNU90)
- GCC and GNU Make
- UNIX/Linux
- UDP Socket Programming
- UBC pthreads
- RT Threads
- xv6
- Git

## Build Instructions

Build all user-space components:

```bash
make
```

Build S-Chat:

```bash
make s-chat
```

Build the readers-writers test:

```bash
make reader_writer_test
```

Clean build artifacts:

```bash
make clean
```

## Running S-Chat

Terminal 1:

```bash
./s-chat 30001 localhost 30002
```

Terminal 2:

```bash
./s-chat 30002 localhost 30001
```

## Architecture Support

The Makefile includes library configurations for x86-64, PowerPC, and ARMv7. Cross-compilation requires the appropriate compiler and target libraries.

## Dependencies

The current build configuration depends on university-provided UBC pthreads and RT Threads libraries. Library paths must be configured when building outside the university environment.

## Project Background

Developed collaboratively as part of CMPT 332 (Operating Systems) at the University of Saskatchewan.

