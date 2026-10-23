# Recording scripts

Read each script aloud and record it on your phone, at normal speaking speed, in a quiet room. Save the files with these exact names in the `Operating Systems` folder:

- `Lecture 01 - recording.m4a`
- `Lecture 02 - recording.m4a`

Each one takes about 90 seconds. Some sentences are planted on purpose so the test has a known correct answer. They are marked here but should be read normally.

---

## Lecture 01 - Processes

Good morning, everyone. Today we start with processes.

A process is simply a program in execution. Your code on disk is just a program. When it runs, it becomes a process, with its own address space, its own registers and its own program counter.

Every process moves through states. It starts as new, then becomes ready. When the scheduler picks it, it is running. If it waits for input or output, it goes to waiting, and when that finishes it goes back to ready, not straight back to running. When it is done, it is terminated.

The operating system keeps all of this in the Process Control Block, the PCB. It stores the state, the program counter, the registers and the list of open files.

> **Planted: lecturer emphasis**
> Please remember this: a context switch means saving the PCB of one process and loading the PCB of another. This is very important, and I will ask it in the exam.

A context switch is pure overhead. No useful work happens while it runs, so we want as few as possible.

That's all for today. Next time, memory management.

---

## Lecture 02 - Memory Management

Today we cover memory management, starting with paging.

In paging, physical memory is split into fixed-size blocks called frames, and logical memory is split into pages of the same size. Any page can go into any free frame, so we avoid external fragmentation.

> **Planted: conflict with the slides (the slides say 4 KB)**
> In today's example we will use a page size of 8 KB.

Every memory access now needs a page table lookup, which is slow. So we add a small cache called the Translation Lookaside Buffer, or TLB. The hit ratio, h, is the fraction of lookups found in the TLB.

The effective access time is h times the TLB time plus memory time, plus one minus h, times the TLB time plus two memory accesses. With a hit ratio of 0.9, a 10 nanosecond TLB and 100 nanosecond memory, that gives 120 nanoseconds.

> **Planted: lecturer emphasis**
> Paging versus segmentation will definitely be in the exam, so make sure you can compare them.

Segmentation divides memory into variable-size logical units, like code, data and stack. We'll compare the two in detail next week.
