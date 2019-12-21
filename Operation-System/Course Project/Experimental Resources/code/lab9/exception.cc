// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void AdvancePC(){
    machine->WriteRegister(PrevPCReg,machine->ReadRegister(PCReg));		// 前一个PC
    machine->WriteRegister(PCReg,machine->ReadRegister(NextPCReg));		// 当前PC
    machine->WriteRegister(NextPCReg,machine->ReadRegister(NextPCReg)+4);	// 下一个PC
}

void StartProcess(int spaceId) {
	// printf("spaceId:%d\n",spaceId);
    ASSERT(currentThread->userProgramId() == spaceId);
    currentThread->space->InitRegisters();     // 设置寄存器初值
    currentThread->space->RestoreState();      // 加载页表寄存器
    machine->Run();             // 运行
    ASSERT(FALSE);
}

void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2);	// 2号寄存器

    if(which == SyscallException){
    	switch(type){
    		case SC_Halt:{
    			printf("This is SC_Halt, CurrentThreadId: %d\n",(currentThread->space)->getSpaceId());
	    		DEBUG('a', "Shutdown, initiated by user program.\n");
			   	interrupt->Halt();
    			break;
    		}
    		case SC_Exec:{
    			printf("This is SC_Exec, CurrentThreadId: %d\n",(currentThread->space)->getSpaceId());
    			int addr = machine->ReadRegister(4);
    			char filename[50];
    			for(int i = 0; ; i++){
    				machine->ReadMem(addr+i,1,(int *)&filename[i]);
    				if(filename[i] == '\0') break;
    			}
    			OpenFile *executable = fileSystem->Open(filename);
    			if(executable == NULL) {
    				printf("Unable to open file %s\n",filename);
    				return;
    			}
    			// 建立新地址空间
    			AddrSpace *space = new AddrSpace(executable);
    			// space->Print();   // 输出新分配的地址空间
    			delete executable;	// 关闭文件
    			// 建立新核心线程
    			Thread *thread = new Thread(filename);
                printf("new Thread, SpaceId: %d, Name: %s\n",space->getSpaceId(),filename);
    			// 将用户线程映射到核心线程上
    			thread->space = space;
    			thread->Fork(StartProcess,(int)space->getSpaceId());
    			machine->WriteRegister(2,space->getSpaceId());
    			AdvancePC();
    			break;
    		}
    		case SC_Join:{
                printf("This is SC_Join, CurrentThreadId: %d\n",(currentThread->space)->getSpaceId());
    			int SpaceId = machine->ReadRegister(4);
    			currentThread->Join(SpaceId);
    			// waitProcessExitCode —— 返回 Joinee 的退出码
    			machine->WriteRegister(2, currentThread->waitExitCode());
    			AdvancePC();
    			break;
    		}
    		case SC_Exit:{
                printf("This is SC_Exit, CurrentThreadId: %d\n",(currentThread->space)->getSpaceId());
    			int exitCode = machine->ReadRegister(4);
    			machine->WriteRegister(2,exitCode);
    			currentThread->setExitCode(exitCode);
    			// 父进程的退出码特殊标记，由 Join 的实现方式决定
    			if(exitCode == 99)
    				scheduler->emptyList(scheduler->getTerminatedList());
   				delete currentThread->space;
   				currentThread->Finish();
                AdvancePC();
    			break;
    		}
            case SC_Yield:{
                printf("This is SC_Yield, CurrentThreadId: %d\n",(currentThread->space)->getSpaceId());
                currentThread->Yield();
                AdvancePC();
                break;
            }
#ifdef FILESYS_STUB
            case SC_Create:{
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i = 0; i < 128; i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                int fileDescriptor = OpenForWrite(filename);
                if(fileDescriptor == -1) printf("create file %s failed!\n",filename);
                else printf("create file %s succeed, the file id is %d\n",filename,fileDescriptor);
                Close(fileDescriptor);
                // machine->WriteRegister(2,fileDescriptor);
                AdvancePC();
                break;
            }
            case SC_Open:{
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i = 0; i < 128; i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                int fileDescriptor = OpenForWrite(filename);
                if(fileDescriptor == -1) printf("Open file %s failed!\n",filename);
                else printf("Open file %s succeed, the file id is %d\n",filename,fileDescriptor);                
                machine->WriteRegister(2,fileDescriptor);
                AdvancePC();
                break;
            }
            case SC_Write:{
                // 读取寄存器信息
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);       // 字节数
                int fileId = machine->ReadRegister(6);      // fd
                
                // 打开文件
                OpenFile *openfile = new OpenFile(fileId);
                ASSERT(openfile != NULL);

                // 读取具体数据
                char buffer[128];
                for(int i = 0; i < size; i++){
                    machine->ReadMem(addr+i,1,(int *)&buffer[i]);
                    if(buffer[i] == '\0') break;
                }
                buffer[size] = '\0';

                // 写入数据
                int writePos;
                if(fileId == 1) writePos = 0;
                else writePos = openfile->Length();
                // 在 writePos 后面进行数据添加
                int writtenBytes = openfile->WriteAt(buffer,size,writePos);
                if(writtenBytes == 0) printf("write file failed!\n");
                else printf("\"%s\" has wrote in file %d succeed!\n",buffer,fileId);
                AdvancePC();
                break;
            }
            case SC_Read:{
                // 读取寄存器信息
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);       // 字节数
                int fileId = machine->ReadRegister(6);      // fd

                // 打开文件读取信息
                char buffer[size+1];
                OpenFile *openfile = new OpenFile(fileId);
                int readnum = openfile->Read(buffer,size);

                for(int i = 0; i < size; i++)
                    if(!machine->WriteMem(addr,1,buffer[i])) printf("This is something Wrong.\n");
                buffer[size] = '\0';
                printf("read succeed, the content is \"%s\", the length is %d\n",buffer,size);
                machine->WriteRegister(2,readnum);
                AdvancePC();
                break;
            }
            case SC_Close:{
                int fileId = machine->ReadRegister(4);
                Close(fileId);
                printf("File %d closed succeed!\n",fileId);
                AdvancePC();
                break;
            }
#else
            case SC_Create:{
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i = 0; i < 128; i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                if(!fileSystem->Create(filename,0)) printf("create file %s failed!\n",filename);
                else printf("create file %s succeed!\n",filename);
                AdvancePC();
                break;
            }
            case SC_Open:{
                int addr = machine->ReadRegister(4), fileId;
                char filename[128];
                for(int i = 0; i < 128; i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                OpenFile *openfile = fileSystem->Open(filename);
                if(openfile == NULL) {
                    printf("File \"%s\" not Exists, could not open it.\n",filename);
                    fileId = -1;
                }
                else{
                    fileId = currentThread->space->getFileDescriptor(openfile);
                    if(fileId < 0) printf("Too many files opened!\n");
                    else printf("file:\"%s\" open succeed, the file id is %d\n",filename,fileId);
                }
                machine->WriteRegister(2,fileId);
                AdvancePC();
                break;
            }
            case SC_Write:{
                // 读取寄存器信息
                int addr = machine->ReadRegister(4);       // 写入数据
                int size = machine->ReadRegister(5);       // 字节数
                int fileId = machine->ReadRegister(6);      // fd
                
                // 创建文件
                OpenFile *openfile = new OpenFile(fileId);
                ASSERT(openfile != NULL);

                // 读取具体写入的数据
                char buffer[128];
                for(int i = 0; i < size; i++){
                    machine->ReadMem(addr+i,1,(int *)&buffer[i]);
                    if(buffer[i] == '\0') break;
                }
                buffer[size] = '\0';


                // 打开文件
                openfile = currentThread->space->getFileId(fileId);
                if(openfile == NULL) {
                    printf("Failed to Open file \"%d\".\n",fileId);
                    AdvancePC();
                    break;
                }
                if(fileId == 1 || fileId == 2){
                    openfile->WriteStdout(buffer,size);
                    delete []buffer;
                    AdvancePC();
                    break;
                }

                // 写入数据
                int writePos = openfile->Length();
                openfile->Seek(writePos);

                // 在 writePos 后面进行数据添加
                int writtenBytes = openfile->Write(buffer,size);
                if(writtenBytes == 0) printf("Write file failed!\n");
                else if(fileId != 1 & fileId != 2)
                    printf("\"%s\" has wrote in file %d succeed!\n",buffer,fileId);
                AdvancePC();
                break;
            }
            case SC_Read:{
                // 读取寄存器信息
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);       // 字节数
                int fileId = machine->ReadRegister(6);      // fd

                // 打开文件
                OpenFile *openfile = currentThread->space->getFileId(fileId);

                // 打开文件读取信息
                char buffer[size+1];
                int readnum = 0;
                if(fileId == 0) readnum = openfile->ReadStdin(buffer,size);
                else readnum = openfile->Read(buffer,size);

                // printf("readnum:%d,fileId:%d,size:%d\n",readnum,fileId,size);
                for(int i = 0; i < readnum; i++)
                    machine->WriteMem(addr,1,buffer[i]);
                buffer[readnum] = '\0';

                for(int i = 0; i < readnum; i++)
                    if(buffer[i] >= 0 && buffer[i] <= 9) buffer[i] = buffer[i]+0x30;
                char *buf = buffer;
                if(readnum > 0){
                    if(fileId != 0)
                        printf("Read file (%d) succeed! the content is \"%s\", the length is %d\n",fileId,buf,readnum);
                }
                else printf("\nRead file failed!\n");
                machine->WriteRegister(2,readnum);
                AdvancePC();
                break;
            }
            case SC_Close:{
                int fileId = machine->ReadRegister(4);
                OpenFile *openfile = currentThread->space->getFileId(fileId);
                if(openfile != NULL) {
                    openfile->WriteBack(); // 将文件写入DISK
                    delete openfile;
                    currentThread->space->releaseFileDescriptor(fileId);
                    printf("File %d closed succeed!\n",fileId);
                }
                else printf("Failed to Close File %d.\n",fileId);
                AdvancePC();
                break;
            }
#endif
    		default:{
    			printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
				break;
			}    		
    	}
    } else {
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
    }
}
