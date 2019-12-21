// openfile.h 
//	Data structures for opening, closing, reading and writing to 
//	individual files.  The operations supported are similar to
//	the UNIX ones -- type 'man open' to the UNIX prompt.
//
//	There are two implementations.  One is a "STUB" that directly
//	turns the file operations into the underlying UNIX operations.
//	(cf. comment in filesys.h).
//
//	The other is the "real" implementation, that turns these
//	operations into read and write disk sector requests. 
//	In this baseline implementation of the file system, we don't 
//	worry about concurrent accesses to the file system
//	by different threads -- this is part of the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef OPENFILE_H
#define OPENFILE_H

#include "copyright.h"
#include "utility.h"

#ifdef FILESYS_STUB			// Temporarily implement calls to 
					// Nachos file system as calls to UNIX!
					// See definitions listed under #else
class OpenFile {
  public:
    OpenFile(int f) { file = f; currentOffset = 0; }	// open the file
    ~OpenFile() { Close(file); }			// close the file

    int ReadAt(char *into, int numBytes, int position) { 
    		Lseek(file, position, 0); 
		return ReadPartial(file, into, numBytes); 
		}	
    int WriteAt(char *from, int numBytes, int position) { 
    		Lseek(file, position, 0); 
		WriteFile(file, from, numBytes); 
		return numBytes;
		}	
    int Read(char *into, int numBytes) {
		int numRead = ReadAt(into, numBytes, currentOffset); 
		currentOffset += numRead;
		return numRead;
    		}
    int Write(char *from, int numBytes) {
		int numWritten = WriteAt(from, numBytes, currentOffset); 
		currentOffset += numWritten;
		return numWritten;
		}

    int Length() { Lseek(file, 0, 2); return Tell(file); }

  private:
    int file;
    int currentOffset;
};

#else // FILESYS
class FileHeader;

class OpenFile {
  public:
    OpenFile(int sector);   // 打开一个文件头在 sector 扇区的文件（DISK）
    ~OpenFile();			// 关闭文件

    void Seek(int position); 	// 定位文件读写位置
    // 读取 numBytes 字节数据到 into 中，返回实际读取字节
    int Read(char *into, int numBytes);
    // 将 from 中 numByters 数据写入 OpenFile 中
    int Write(char *from, int numBytes);
    // 从 OpenFile 的 pos 位置读取字节到 into 中
    int ReadAt(char *into, int numBytes, int position);
    // 从 from 中的 pos 位置读取字节到 OpenFile 中 
    int WriteAt(char *from, int numBytes, int position);
    int Length();       // 返回文件字节数
    void WriteBack();   // 将文件头写回 DISK 中

#ifdef FILESYS
    OpenFile(char *type) {}
    int WriteStdout(char *from, int numBytes);
    int ReadStdin(char *into, int numBytes);
#endif
    
  private:
    FileHeader *hdr;	// 文件头
    int seekPosition, hdrSector; // 文件当前读取位置，文件头所在扇区号
};

#endif // FILESYS

#endif // OPENFILE_H
