Task：同步机制

+ threads / synch.h / synch.cc
+ threads / synchlist.cc



## code rule in c++

#### 1. extern

This comes in useful when you have global variables. You declare the *existence* of global variables using "extern" in a header, so that each source file that includes the header knows about it, but you only need to “define” it once in one of your source files.

To clarify, using `extern int x;` tells the compiler that an object of type `int` called `x` exists *somewhere*. It's not the compilers job to know where it exists, it just needs to know the type and name so it knows how to use it. Once all of the source files have been compiled, **the linker will resolve all of the references of `x`** to the one definition that it finds in one of the compiled source files.



#### 2. ASSERT

```cpp
define ASSERT(condition)

	if(!(condition)){
    fprintf(...);
    Abort();
  }
```



#### 3. thread

thread 私有变量中存储了 `int* stack` 以及 `ThreadStatus status` 

> $Sleep()$

$status$ 为 $Thread$ 的私有成员，表示进程ready、running、blocked三种状态

$Sleep()$ 函数表示当前进程被阻塞，并调度新的进程来运行

$Scheduler$ 类中维护了一个 $readyList$，存储所有 $ready$ 状态的 $thread$，$Run(thread)$ 函数即为运行该 $thread$。

$scheduler\rightarrow FindNextToRun()$ 运行了 $readyList$ 的 $Remove$ 函数，即将 $List$ 中第一个元素删除，并返回第一个元素，先入先出模型。

```cpp
//----------------------------------------------------------------------
// Thread::Sleep
// 	Relinquish the CPU, because the current thread is blocked
//	waiting on a synchronization variable (Semaphore, Lock, or Condition).
//	Eventually, some thread will wake this thread up, and put it
//	back on the ready queue, so that it can be re-scheduled.
//
//	NOTE: if there are no threads on the ready queue, that means
//	we have no thread to run.  "Interrupt::Idle" is called
//	to signify that we should idle the CPU until the next I/O interrupt
//	occurs (the only thing that could cause a thread to become
//	ready to run).
//
//	NOTE: we assume interrupts are already disabled, because it
//	is called from the synchronization routines which must
//	disable interrupts for atomicity.   We need interrupts off 
//	so that there can't be a time slice between pulling the first thread
//	off the ready list, and switching to it.
//----------------------------------------------------------------------

void Thread::Sleep ()
{
    Thread *nextThread;
    
    ASSERT(this == currentThread);
    ASSERT(interrupt->getLevel() == IntOff);
    
    DEBUG('t', "Sleeping thread \"%s\"\n", getName());

    status = BLOCKED;
    while ((nextThread = scheduler->FindNextToRun()) == NULL)
	interrupt->Idle();	// no one to run, wait for an interrupt
        
    scheduler->Run(nextThread); // returns when we've been signalled
}
```



#### 4. Scheduler

> ReadyToRun (Thread *thread)

将 thread 的状态设为READY，并将其放入 readyList 中。



![image-20191106131603846](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191106131603846.png)



![image-20191111040340775](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191111040340775.png)



#### 5. DEBUG

```cpp
/* 在 ../threads/utility.cc 中 */

//	The debugging routines allow the user to turn on selected
//	debugging messages, controllable from the command line arguments
//	passed to Nachos (-d).  You are encouraged to add your own
//	debugging flags.  The pre-defined debugging flags are:
//
//	'+' -- turn on all debug messages
//   	't' -- thread system
//   	's' -- semaphores, locks, and conditions
//   	'i' -- interrupt emulation
//   	'm' -- machine emulation (USER_PROGRAM)
//   	'd' -- disk emulation (FILESYS)
//   	'f' -- file system (FILESYS)
//   	'a' -- address spaces (USER_PROGRAM)
//   	'n' -- network emulation (NETWORK)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

//----------------------------------------------------------------------
// DebugInit
//      Initialize so that only DEBUG messages with a flag in flagList 
//	will be printed.
//
//	If the flag is "+", we enable all DEBUG messages.
//
// 	"flagList" is a string of characters for whose DEBUG messages are 
//		to be enabled.
//----------------------------------------------------------------------

void DebugInit(char *flagList)
{
    enableFlags = flagList; //enableFlags 由 flagList 定义
}

bool DebugIsEnabled(char flag) {
  // strchr(char *s, char c)，搜索 c 在 s 中第一次出现的位置
    if (enableFlags != NULL)
       return (bool)((strchr(enableFlags, flag) != 0) 
                     || (strchr(enableFlags, '+') != 0));
    else return FALSE;
}

void DEBUG(char flag, char *format, ...) {
  if (DebugIsEnabled(flag)) { // 判断 flag 是否可以打开 Debug
    va_list ap; 
    // You will get an unused variable message here -- ignore it.
		va_start(ap, format);
    vfprintf(stdout, format, ap); // 输出
    va_end(ap);
    fflush(stdout);
	}
}
```



#### 6. main.cc

```cpp
int main(int argc, char **argv);
/* 其中 argc 表示命令参数，argv 大小为 argc，存储每个命令的字符串形式 */
```

![image-20191120150251907](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191120150251907.png)





#### 7. void Initialize(int argc, char **argv)

```cpp
for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount) {
	argCount = 1;
	if (!strcmp(*argv, "-d")) {
	    if (argc == 1)
		debugArgs = "+";	// turn on all debug flags
	    else {
	    	debugArgs = *(argv + 1);
	    	argCount = 2;
	    }
	} else if (!strcmp(*argv, "-rs")) {
	    ASSERT(argc > 1);     // 设置 -rs 时必须设置随机数种子
	    // 传入计数器种子
        RandomInit(atoi(*(argv + 1)));	// initialize pseudo-random
						// number generator
	    randomYield = TRUE;
	    argCount = 2;
	}
```

+ -d
  + 如果只是-d，则设置为打开所有debug flags
  + 如果是 -d t，则只打开 thread 的 debug 部分
+ -rs
  + -rs 一定要设置随机数种子，不然会报错（从代码中ASSERT中可以看到）

![image-20191120151344219](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191120151344219.png)





#### 8. stats.h

![image-20191120151945678](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191120151945678.png)



#### 9. 时间片

![image-20191127145059877](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191127145059877.png)







## synch.h

Three kinds of synchronization:

1. semaphores
2. locks
3. condition variables

#### 1. Head File

```cpp
/* 
synch.h
	Data structures for synchronizing threads.
	
	Three kinds of synchronization are defined here: semaphores, locks, and condition variables. The implementation for semaphores is given; for the latter two, only the procedure interface is given -- they are to be implemented as part of the first assignment.
	
	Note that all the synchronization objects take a "name" as part of the initialization. This is solely for debugging purposes.
*/

#ifndef SYNCH_H     // to test if SYNCH_H is defined
#define SYNCH_H			// if not defined before, access to this part

#include "copyright.h"
#include "thread.h"
#include "list.h"
```

#### 2. Semaphore

+ Two ops, $P()$ and $V()$ should be atomic. 
+ Not allow to read the semaphore value (even if read, it is just before)

```cpp
/*
	The following class defines a "semaphore" whose value is a non-negative integer. The semaphore has only two operations P() and V():
		P() ———— waits until value > 0, then decrement
		V() ———— increment, waking up a thread waiting in P() if necessary
		
	Note that the interface **does not allow a thread to read the value of the semaphore directly** -- even if you did read the value, the only thing you would know is what the value used to be. You don't know what the value is now, because by the time you get the value into a register, a context switch might have occurred, and some other thread might have called P or V, so the true value might now be different.
*/

class Semaphore {
  public:
  	Semaphore(char* debugName, int initialValue);		//set initial value
  	~Semaphore();																		//de-allocate semaphore
  	char* getName() { return name; }								//debugging assist
  
  	void P();		//these are the only operations on a semaphore
  	void V();		//they are both atomic
 	
  private:
  	char* name;		//useful for debugging
  	int value;		//semaphore value, always >= 0
  	List *queue;	//threads waiting in P() for the value to be > 0
};
```

#### 3. Lock

+ two ops, $Acquire()$ and $Release()$
+ not allow to read the lock value

```cpp
/*
	The following class defines a "lock". A lock can be BUSY or FREE.
  There are only two operations allowed on a lock:
  	Acquire -- wait until the lock is FREE, then set it to BUSY
  	Release -- set lock to FREE, waking up a thread waiting in Acquire if necessary
  	
  In addition, by convention（按惯例）, only the thread that acquired the lock may release it. As with semaphores, you can't read the lock value（锁值不可查看） (because the value might change immediately after you read it).
*/

class Lock{
  public:
  	Lock(char* debugName);							//initialize lock to be FREE
  	~Lock();														//de-allocate lock
  	char* getName() { return name; } 		//debugging assist

  	void Acquire();				//these are the only operation on a lock
  	void Release();				//they are both atomic.
  
  	//Useful for checking in Release, and in Condition variable ops below.（对下述操作有效）
  	bool isHeldByCurrentThread();		//true if the current thread holds this lock.

  private:
  	char* name;							//for debugging
  	Thread *owner;					//remember who acquired the lock
  	Semaphore *lock;				//use semaphore for the actual lock
};
```

#### 4. Condition Variable

+ three ops, $Wait()$, $Signal()$ and $Broadcast()$
+ Only when the thread has a lock, these ops can be performed
+ Only when threads have same lock can they access to same condition variable



\*Mesa\*-style semantics 

+ wakes up a thread, then puts the thread on the ready list

+ the woken thread need to re-acquire the lock

   

\*Hoare\*-style semantics 

+ the signalling thread gives up lock and CPU to the woken thread
+ when the woken thread leaves the critical section, it gives back control to the signaler

```cpp
/*
	The following class defines a "condition variable". A condition variable does not have a value, but threads may be queued, waiting on the variable. These are only operations on a condition variable:
		Wait() -- release the lock, relinquish (放弃) the CPU until signaled, then re-acquire the lock
		Signal() -- wake up a thread, if there are any waiting on the condition
		Broadcast() -- wake up all threads waiting on the condition

	All operations on a condition variable must be made while the current thread has acquired a lock. Indeed, all accesses to a given condition variable must be protected by the same lock. In other words , mutual exclusion must be enforced among threads calling the condition variable operations.
	
	In Nachos, condition variables are assumed to obey *Mesa*-style semantics. When a Signal or Broadcast wakes up another thread, it simply puts the thread on the ready list, and it is the responsibility of the woken thread to re-acquire the lock (this re-acquire is taken care of within Wait()). 
	By contrast, some define condition variables according to *Hoare*-style semantics -- where the signalling thread gives up control over the lock and the CPU to the woken thread, which runs immediately and gives back control over the lock to the signaler when the woken thread leaves the critical section (临界区).
	
	The consequence of using Mesa-style semantics is that some other thread can acquire the lock, and change data structures, before the woken thread gets a chance to run.
*/

class Condition{
  public:
  	Condition(char* debugName);		//initialize condition to "no one waiting"
  	~Condition();									//deallocate the condition
  	char* getName() { return (name); }
  	
  	//these are the 3 ops on condition variables
  	//conditionLock must be held by the currentThread for all of these ops
  	void Wait(Lock *conditionLock);		//releasing the lock and going to sleep are *atomic* in Wait()
  	void Signal(Lock *conditionLock);
  	void Broadcast(Lock *conditionLock);
  
  private:
  	char* name;
  	List* queue; //threads waiting on the condition
  	Lock* lock; //debugging aid: used to check correctness of arguments to Wait, Signal and Broadcast
};
# endif //SYNCH_H
```



## synch.cc

#### 1. Head File

+ uniprocessor, keep synchronization through turning off/on interrupts
+ Re-set the interrupt state back to its original value at the end of the atomic operation 

```cpp
/*
synch.cc
	Routines for synchronizing threads. Three kinds of synchronization routines are defined here: semaphores, locks and condition variables (the implementation of the last two are left to the reader).
	
	(单处理器通过开关中断来保持操作原子性，要求线程能够一直占有CPU直到中断恢复)
	Any implementation of a synchronization routine needs some primitive atomic operation. We assume Nachos is running on a uniprocessor (单处理器), and thus atomicity can be provided by turning off interrupts. While interrupts are disabled, no context switch can occur, and thus the current thread is guaranteed to hold the CPU throughout, until interrupts are reenabled.
	
	Because some of these routines might be called with interrupts already disabled (Semaphore::V for one), instead of turning on interrupts at the end of the atomic operation, we always simply re-set the interrupt state back to its original value (whether that be disabled or enabled).
*/

#include "copyright.h"
#inlude "sysch.h"
#include "system.h"
```

#### 2. Semaphore

+ $P()$ and $V() $ should be atomic.

```cpp
/*-------------- --------------------------------------------------------------
Semaphore::Semaphore
  Initialize a semaphore, so that it can be used for synchronization.
    
  "debugName" is an arbitrary name, useful for debugging.
  "InitialValue" is the initial valuue of the semaphore.
/----------------------------------------------------------------------------*/
Semaphore::Semaphore(char* debugName, int initialValue)
{
  name = debugName;
  value = initialValue;
  queue = new List;			//new empty queue
}

/*----------------------------------------------------------------------------
Semaphore::~Semaphore
  De-allocate semaphore, when no longer needed. Assume no one is still waiting on the semaphore!
/----------------------------------------------------------------------------*/

Semaphore::~Semaphore()
{
  delete queue;
}

/*----------------------------------------------------------------------------
Semaphore::P
  Wait until semaphore value > 0, then decrement. Checking the value and decrementing must be done atomically, so we need to disable interrupts before checking the values.
  
  Note that Thread::Sleep assumes that interrupts are disabled when it is called.
  
  IntStatus定义在interrupt.h中:
  	enum IntStatus { IntOff, IntOn };
  
  SetLevel在定义interrupt.cc中, 用于设置当前中断状态.
  
  currentThread定义在system.h中，representing the thread holding the CPU
  
  Sleep()定义在thread.cc中，发生在关中断时，使当前进程被阻塞，等待新的可运行进程出现
/----------------------------------------------------------------------------*/

void Semaphore::P()
{
  IntStatus oldLevel = interrupt->SetLevel(IntOff);			//disable interrupts
  
  while(value == 0){										//semaphore not available, so go to sleep
    queue->Append((void*)currentThread);
    currentThread->Sleep();
  }
  value--;			//semaphore available, consume its value
  (void)interrupt->SetLevel(oldLevel);	//re-enable interrupts
}

/*----------------------------------------------------------------------
	Semaphore::V
		Increment semaphore value, waking up a waiter if necessary.
		As with P(), this operation must be atomic, so we need to disable
		interrupts.  Scheduler::ReadyToRun() assumes that threads
		are disabled when it is called.
/----------------------------------------------------------------------*/

void Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);		//关中断并返回旧状态

    thread = (Thread *)queue->Remove();	//取出等待队列中的第一个
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);	//进程状态置为ready，并放入readyList中等待运行，readyList is a queue of threads that are ready to run, but not running
    value++;
    (void) interrupt->SetLevel(oldLevel);	//开中断
}


```







## 信号量

一种可动态定义的软件资源：信号量

+ 核心数据结构：等待进程队列
+ 信号量声明：资源报到，建立队列
+ 申请资源的原语：若申请不得，调用进程入队等待
+ 归还资源的原语：若队列中有等待进程，需释放
+ 信号量撤销：资源注销，撤销队列



#### 记录型信号量

+ 每个信号量建立一个等待进程队列
+ 每个信号量相关一个整数值
  + 正值表示资源可复用次数，临界资源则为1
  + 0值表示无资源且无进程等待
  + 负值表示等待队列中进程个数

![image-20191111054250385](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191111054250385.png)

P(s)

临界区

V(s)