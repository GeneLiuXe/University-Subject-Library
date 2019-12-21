// thread.h 
//	Data structures for managing threads.  A thread represents
//	sequential execution of code within a program.
//	So the state of a thread includes the program counter,
//	the processor registers, and the execution stack.
//	
// 	Note that because we allocate a fixed size stack for each
//	thread, it is possible to overflow the stack -- for instance,
//	by recursing to too deep a level.  The most common reason
//	for this occuring is allocating large data structures
//	on the stack.  For instance, this will cause problems:
//
//		void foo() { int buf[1000]; ...}
//
//	Instead, you should allocate all data structures dynamically:
//
//		void foo() { int *buf = new int[1000]; ...}
//
//
// 	Bad things happen if you overflow the stack, and in the worst 
//	case, the problem may not be caught explicitly.  Instead,
//	the only symptom may be bizarre segmentation faults.  (Of course,
//	other problems can cause seg faults, so that isn't a sure sign
//	that your thread stacks are too small.)
//	
//	One thing to try if you find yourself with seg faults is to
//	increase the size of thread stack -- ThreadStackSize.
//
//  	In this interface, forking a thread takes two steps.
//	We must first allocate a data structure for it: "t = new Thread".
//	Only then can we do the fork: "t->fork(f, arg)".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef THREAD_H
#define THREAD_H

#include "copyright.h"
#include "utility.h"

#ifdef USER_PROGRAM
#include "machine.h"
#include "addrspace.h"
#endif


// CPU register state to be saved on context switch.  
// The SPARC and MIPS only need 10 registers, but the Snake needs 18.
// For simplicity, this is just the max over all architectures.
#define MachineStateSize 18

// Size of the thread's private execution stack.
// WATCH OUT IF THIS ISN'T BIG ENOUGH!!!!!
#define StackSize	(sizeof(_int) * 1024)	// in words

// 线程状态
enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED, TERMINATED };

// external function, dummy routine whose sole job is to call Thread::Print
extern void ThreadPrint(_int arg);	 

/*
    该类定义了线程控制块。
    每个线程都拥有（1）线程执行栈（2）数组存储CPU寄存器状态（3）线程状态（运行、可运行、阻塞）
    用户线程拥有用户地址空间，仅运行在内核态的线程没有地址空间
*/

class Thread {
  private:
    // 下述两个变量用于上下文切换，位置不可更改
    int* stackTop;			 // 当前栈指针
    _int machineState[MachineStateSize];  // 所有CPU寄存器状态

    int* stack;             // 栈底指针，主线程栈底指针为NULL
    char *name;
    ThreadStatus status;    // 线程状态（运行、可运行、阻塞）
    // 为线程分配栈空间，用于Fork函数内部实现
    void StackAllocate(VoidFunctionPtr func, _int arg);

#ifdef USER_PROGRAM
    // 运行用户程序的线程有两组CPU寄存器，一个存储运行用户代码的线程状态，一个存储运行内核代码的线程状态

    int userRegisters[NumTotalRegs];    // 用户态下CPU寄存器状态
    int waitProcessSpaceId, waitProcessExitCode, exitCode;    
#endif

  public:
    AddrSpace *space;			// 运行用户程序时的地址空间
    Thread(char* debugName);		// 构造函数 
    ~Thread(); 				// 析构函数，运行态线程不可析构
    
    // 下述为基础线程操作
    void Fork(VoidFunctionPtr func, _int arg); 	// 使线程运行在 (*func)(arg) 函数位置
    void Yield();  		// 当前线程，运行态 => 可运行态，调度其它线程
    void Sleep();  		// 当前线程，运行态 => 阻塞态，调度其它线程
    void Finish();  	// 线程运行结束
    
    void CheckOverflow();   	// 检查线程栈是否溢出
    void setStatus(ThreadStatus st) { status = st; }    // 设置线程状态
    char* getName() { return (name); }      // 获取线程名
    void Print() { printf("%s\n", name); }  // 输出线程名

#ifdef USER_PROGRAM
    void SaveUserState();		// 保存用户态下寄存器状态
    void RestoreUserState();	// 恢复用户态下寄存器状态
    void Join(int SpaceId);
    void Terminated();
    int userProgramId() { return space->getSpaceId(); }
    int ExitCode() { return exitCode; }
    int waitExitCode() { return waitProcessExitCode; }
    int setWaitExitCode(int tmpCode) { waitProcessExitCode = tmpCode; }
    int setExitCode(int tmpCode) { exitCode = tmpCode; }
#endif
};

// Magical machine-dependent routines, defined in switch.s

extern "C" {
// First frame on thread execution stack; 
//   	enable interrupts
//	call "func"
//	(when func returns, if ever) call ThreadFinish()
void ThreadRoot();

// Stop running oldThread and start running newThread
void SWITCH(Thread *oldThread, Thread *newThread);
}

#endif // THREAD_H
