// scheduler.h 
//	Data structures for the thread dispatcher and scheduler.
//	Primarily, the list of threads that are ready to run.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "copyright.h"
#include "list.h"
#include "thread.h"

// The following class defines the scheduler/dispatcher abstraction -- 
// the data structures and operations needed to keep track of which 
// thread is running, and which threads are ready but not running.

class Scheduler {
  public:
    Scheduler();			// 初始化调度队列
    ~Scheduler();			// 析构函数

    void ReadyToRun(Thread* thread);	// 将线程放入可运行队列
    Thread* FindNextToRun();		  // 找到第一个可运行态线程
    void Run(Thread* nextThread);	// 运行线程
    void Print();			// 打印可运行线程队列
    
  private:
    List *readyList;      // 可运行态线程的队列

#ifdef USER_PROGRAM
  public:
    List *getReadyList() { return readyList; }
    List *getWaitingList() { return waitingList; }
    List *getTerminatedList() { return terminatedList; }
    void deleteTerminatedThread(int deleteSpaceId);
    void emptyList(List *tmpList) { delete tmpList; }
  private:
    List *waitingList;    // 等待运行线程的队列
    List *terminatedList; // 终止运行但未释放线程的队列
#endif
};

#endif // SCHEDULER_H
