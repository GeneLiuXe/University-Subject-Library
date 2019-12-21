// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------
#define MAX_USERPROCESSES 256

BitMap *AddrSpace::userMap = new BitMap(NumPhysPages);
BitMap *AddrSpace::pidMap = new BitMap(MAX_USERPROCESSES);

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable) {
    ASSERT(pidMap->NumClear() >= 1);  // 保证还有线程号可以分配
    spaceId = pidMap->Find()+100;     // 0-99留给内核线程

    // 可执行文件中包含了目标代码文件
    NoffHeader noffH;               // noff文件头
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);   // 读出noff文件
    if ((noffH.noffMagic != NOFFMAGIC) && (WordToHost(noffH.noffMagic) == NOFFMAGIC))
        SwapHeader(&noffH);           // 检查noff文件是否正确
    ASSERT(noffH.noffMagic == NOFFMAGIC);
    // 确定地址空间大小，其中还包括了用户栈大小
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;   
    numPages = divRoundUp(size, PageSize);  // 确定页数
    size = numPages * PageSize;             // 计算真实占用大小
    ASSERT(numPages <= NumPhysPages);       // 确认运行文件大小可以运行

    DEBUG('a', "Initializing address space, num pages %d, size %d\n", numPages, size);
    // 第一步，创建页表，并对每一页赋初值
    pageTable = new TranslationEntry[numPages];

    ASSERT(userMap->NumClear() >= numPages);    // 确认页面足够分配 
    for (i = 0; i < numPages; i++) {
      pageTable[i].virtualPage = i;         // 虚拟页
      pageTable[i].physicalPage = userMap->Find();  // 在位图找空闲页进行分配
      pageTable[i].valid = TRUE;
      pageTable[i].use = FALSE;
      pageTable[i].dirty = FALSE;
      pageTable[i].readOnly = FALSE;        // 只读选项
    }
    // 第二步，将noff文件数据拷贝到物理内存中
    if (noffH.code.size > 0) {
      int pagePos = pageTable[noffH.code.virtualAddr/PageSize].physicalPage * PageSize;
      int offSet = noffH.code.virtualAddr % PageSize;

      executable->ReadAt(&(machine->mainMemory[pagePos+offSet]),
        noffH.code.size, noffH.code.inFileAddr); // ReadAt调用了bcopy函数
    }
    if (noffH.initData.size > 0) {
      int pagePos = pageTable[noffH.initData.virtualAddr/PageSize].physicalPage * PageSize;
      int offSet = noffH.initData.virtualAddr % PageSize;

      executable->ReadAt(&(machine->mainMemory[pagePos+offSet]),
                noffH.initData.size, noffH.initData.inFileAddr);
    }
#ifdef FILESYS
    for(int i = 3; i < 10; i++) fileDescriptor[i] = NULL;
    OpenFile *StdinFile = new OpenFile("stdin");
    OpenFile *StdoutFile = new OpenFile("stdout");
    /* 保持怀疑 */
    fileDescriptor[0] = StdoutFile;
    fileDescriptor[1] = StdoutFile;
    fileDescriptor[2] = StdoutFile;
#endif
}

#ifdef FILESYS
int AddrSpace::getFileDescriptor(OpenFile *openfile) {
  for(int i = 3; i < 10; i++)
    if(fileDescriptor[i] == NULL){
      fileDescriptor[i] = openfile;
      return i;
    }
  return -1;
}

OpenFile* AddrSpace::getFileId(int fd) {
  return fileDescriptor[fd];
}

void AddrSpace::releaseFileDescriptor(int fd) {
  fileDescriptor[fd] = NULL;
}
#endif

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
  pidMap->Clear(spaceId-100);
  for(int i = 0; i < numPages; i++)
    userMap->Clear(pageTable[i].physicalPage);
  delete [] pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{
  pageTable = machine->pageTable;
  numPages = machine->pageTableSize;
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}


void AddrSpace::Print() {
    printf("page table dump: %d pages in total\n",numPages);
    printf("============================================\n");
    printf("\tVirtPage, \tPhysPage\n");

    for(int i = 0; i < numPages; i++)
        printf("\t%d,\t\t%d\n",pageTable[i].virtualPage,pageTable[i].physicalPage);
    printf("============================================\n\n");
}