// directory.cc 
//	Routines to manage a directory of file names.
//
//	The directory is a table of fixed length entries; each
//	entry represents a single file, and contains the file name,
//	and the location of the file header on disk.  The fixed size
//	of each directory entry means that we have the restriction
//	of a fixed maximum size for file names.
//
//	The constructor initializes an empty directory of a certain size;
//	we use ReadFrom/WriteBack to fetch the contents of the directory
//	from disk, and to write back any modifications back to disk.
//
//	Also, this implementation has the restriction that the size
//	of the directory cannot expand.  In other words, once all the
//	entries in the directory are used, no more files can be created.
//	Fixing this is one of the parts to the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "utility.h"
#include "filehdr.h"
#include "directory.h"

//----------------------------------------------------------------------
// Directory::Directory
// 	Initialize a directory; initially, the directory is completely
//	empty.  If the disk is being formatted, an empty directory
//	is all we need, but otherwise, we need to call FetchFrom in order
//	to initialize it from disk.
//
//	"size" is the number of entries in the directory
//----------------------------------------------------------------------

Directory::Directory(int size)
{
    table = new DirectoryEntry[size];
    tableSize = size;
    for (int i = 0; i < tableSize; i++)
	table[i].inUse = FALSE;
}

//----------------------------------------------------------------------
// Directory::~Directory
// 	De-allocate directory data structure.
//----------------------------------------------------------------------

Directory::~Directory()
{ 
    delete [] table;
} 

//----------------------------------------------------------------------
// Directory::FetchFrom
// 	Read the contents of the directory from disk.
//
//	"file" -- file containing the directory contents
//----------------------------------------------------------------------

void
Directory::FetchFrom(OpenFile *file)
{
    (void) file->ReadAt((char *)table, tableSize * sizeof(DirectoryEntry), 0);
}

//----------------------------------------------------------------------
// Directory::WriteBack
// 	Write any modifications to the directory back to disk
//
//	"file" -- file to contain the new directory contents
//----------------------------------------------------------------------

void
Directory::WriteBack(OpenFile *file)
{
    (void) file->WriteAt((char *)table, tableSize * sizeof(DirectoryEntry), 0);
}

//----------------------------------------------------------------------
// Directory::FindIndex
// 	Look up file name in directory, and return its location in the table of
//	directory entries.  Return -1 if the name isn't in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int
Directory::FindIndex(char *name)
{
    for (int i = 0; i < tableSize; i++)
        if (table[i].inUse && !strncmp(table[i].name, name, FileNameMaxLen))
	    return i;
    return -1;		// name not in directory
}

//----------------------------------------------------------------------
// Directory::Find
// 	Look up file name in directory, and return the disk sector number
//	where the file's header is stored. Return -1 if the name isn't 
//	in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int
Directory::Find(char *name)
{
    int i = FindIndex(name);

    if (i != -1)
	return table[i].sector;
    return -1;
}

//----------------------------------------------------------------------
// Directory::Add
// 	Add a file into the directory.  Return TRUE if successful;
//	return FALSE if the file name is already in the directory, or if
//	the directory is completely full, and has no more space for
//	additional file names.
//
//	"name" -- the name of the file being added
//	"newSector" -- the disk sector containing the added file's header
//----------------------------------------------------------------------

bool
Directory::Add(char *name, int newSector)
{ 
    if (FindIndex(name) != -1)
	return FALSE;

    for (int i = 0; i < tableSize; i++)
        if (!table[i].inUse) {
            table[i].inUse = TRUE;
            strncpy(table[i].name, name, FileNameMaxLen); 
            table[i].sector = newSector;
        return TRUE;
	}
    return FALSE;	// no space.  Fix when we have extensible files.
}

//----------------------------------------------------------------------
// Directory::Remove
// 	Remove a file name from the directory.  Return TRUE if successful;
//	return FALSE if the file isn't in the directory. 
//
//	"name" -- the file name to be removed
//----------------------------------------------------------------------

bool
Directory::Remove(char *name)
{ 
    int i = FindIndex(name);

    if (i == -1)
	return FALSE; 		// name not in directory
    table[i].inUse = FALSE;
    return TRUE;	
}

// #ifdef FILESYS
// extern char *setStrLength(char *str, int len);
// extern char *numFormat(int num);

// void Directory::List() {
//     FileHeader *hdr = new FileHeader;
//     int size = 0;
//     printf("=======================================\n");
//     printf("Name        Size    Sectors     SectorList\n");
//     printf("---------------------------------------\n");
//     for(int i = 0; i < tableSize; i++)
//         if(table[i].inUse) {
//             hdr->FetchFrom(table[i].sector);
//             size += hdr->getFileSize();
//             char *fileName = table[i].name;
//             int fileSize = hdr->getFileSize();
//             int fileSectors = hdr->getNumSectors();
//             printf("%s ",setStrLength(fileName,9));
//             printf("%s ",setStrLength(numFormat(fileSize),7));
//             printf("%s",setStrLength(numFormat(fileSectors),6));
//             printf("%s\n",hdr->getDataSectors());
//         }
//     printf("----------------------------------------\n");
//     BitMap *freeMap = fileSystem->getBitMap();
//     int freeSize = freeMap->NumClear() * 128;
//     printf("Available Disk Space: %s bytes",numFormat(freeSize));
//     printf("(%s KB)\n\n",numFormat(freeSize/1024));
//     delete hdr;
// }

// // delete all char 'c' in str
// char buf[128];
// char *EraseStrChar(char *str,char c) {
//     int i = 0;
//     while (*str != '\0') { 
//         if (*str != c) { 
//             buf[i] = *str;
//             str++;
//             i++; 
//         }
//         else str++;
//     }
//     buf[i] = '\0'; 
//     return buf;
// }

// // Convert integer to str
// char istr[10] = "";
// char* itostr(int num) {
//     int n;
//     char ns[10];
//     int k = 0;
//     while (true) {
//         n = num % 10;
//         ns[k] = 0x30 + n;
//         k++;
//         num = num /10;
//         if (num == 0) break;
//     }
//     for (int i = 0; i < k; i++) 
//         istr[i] = ns[k‐i‐1];
//     return istr;
// }

// /* 
//     set a string to fix length, with blank append at the end of it, 
//     or truncate it to the fix length parametors: str, len
// */
// char strSpace[10];
// char *setStrLength(char *str, int len) {
//     int strLength = strlen(str);
//     if (strLength >= len) {
//         for (int i = 0; i < len ; i++, str++)
//             strSpace[i] = *str; 
//     } else {
//         for (int i = 0; i < strLength ; i++, str++)
//             strSpace[i] = *str;
//         for (int i = strLength; i < len ; i++)
//             strSpace[i] = ' ';
//     }
//     strSpace[len] = '\0';
//     return strSpace;
// }

// // divide an integer with ",", such as int (1,234,567) to char * (1,234,567)
// char numStr[10];
// char *numFormat(int num) {
//     numStr[0] = '\0';
//     char nstr[10]="";
//     int k = 0;
//     while (true) {
//         nstr[k++] = num % 10 + 0x30;
//         num = num /10;
//         if (num == 0 ) break; 
//     }
//     nstr[k] = '\0';
//     int j = k-1;
//     if (strlen(nstr)>=4 && strlen(nstr)<=6) j = k;
//     else if (strlen(nstr)>=7 && strlen(nstr)<=9) j = k + 1; 
//     else if (strlen(nstr)>=10 && strlen(nstr)<=12) j = k + 2;
//     numStr[j+1] = '\0'; for (int i=0;i<k;i++) {
//         if (i>0 && i%3==0) {
//             numStr[j--] = ',';
//             numStr[j--] = nstr[i];
//         } 
//         else numStr[j--] = nstr[i];
//     }
//     return numStr;
// }
// #else
//----------------------------------------------------------------------
// Directory::List
// 	List all the file names in the directory. 
//----------------------------------------------------------------------

void
Directory::List()
{
   for (int i = 0; i < tableSize; i++)
	if (table[i].inUse)
	    printf("%s\n", table[i].name);
}

// #endif

//----------------------------------------------------------------------
// Directory::Print
// 	List all the file names in the directory, their FileHeader locations,
//	and the contents of each file.  For debugging.
//----------------------------------------------------------------------

void
Directory::Print()
{ 
    FileHeader *hdr = new FileHeader;

    printf("Directory contents:\n");
    for (int i = 0; i < tableSize; i++)
	if (table[i].inUse) {
	    printf("Name: %s, Sector: %d\n", table[i].name, table[i].sector);
	    hdr->FetchFrom(table[i].sector);
	    hdr->Print();
	}
    printf("\n");
    delete hdr;
}
