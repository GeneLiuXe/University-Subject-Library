## 一、实验基础信息

##### 个人信息

201700130011 — 刘建东 — 17级菁英班

##### 实验信息

日期：2019.11.28

题目：Nachos 的文件系统、扩展 Nachos 的文件系统

##### 实验目的

（实验四）

1. 理解 $Nachos$ 硬盘是如何创建的。
2. 熟悉查看 $Nachos$ 硬盘上的内容的方法。
3. 理解硬盘初始化的过程（如何在硬盘上创建一个文件系统）。
4. 了解 $Nachos$ 文件系统提供了哪些命令，哪些命令已经实现，哪些需要你自己实现。
5. 理解已经实现的文件系统命令的实现原理。
6. 理解硬盘空闲块的管理方法。
7. 理解目录文件的结构与管理。
8. 理解文件的结构与文件数据块的分配方法。
9. 了解一个文件系统命令执行后，硬盘的布局。
10. 分析目前 $Nachos$ 不能对文件进行扩展的原因，考虑解决方法。

（实验五）

1. 理解文件系统中文件操作的实现方法，如文件打开、读、写、扩展、定位、关闭等。
2. 理解如何管理硬盘空闲块。
3. 创建文件时，如何为文件分配目录项及文件头（FCB）。
4. 理解文件扩展时，如何为要扩展的数据查找并分配空闲块。
5. 理解文件扩展后，文件大小是如何记录与保存的。
6. 文件被删除后，如何回收为其分配的资源，如文件头、目录项、硬盘块等。

##### 实验任务

（实验四）

+ *阅读代码*：../filesys/fstest.cc、synchdisk.cc、openfile.cc、filesys.cc、directory.cc、filehdr.cc，../threads/main.cc、../machine/disk.cc、../userprog/bitmap.cc
+ *分析代码*
  
  1. ../lab5/main.cc 调用了 ../threads/system.cc 中的 Initialize() 创建了硬盘 DISK。分析 ../filesys/synchdisk.cc 及 ../machine/disk.cc，理解 Nachos 创建硬盘的过程与方法。
  
  2. 分析 ../lab5/main.cc，了解 Nachos 文件系统提供了哪些命令，对每个命令进行测试，根据执行结果观察哪些命令已经实现（正确运行），哪些无法正确运行（尚未完全实现，需要你自己完善）。
  
     分析 ../lab5/fstest.cc 及 ../filesys/filesys.cc，理解 Nachos 对这些命令的处理过程与方法。
  
  3. 分析 ../filesys/filesys.cc，特别是构造函数 FileSystem::FileSystem(..)，理解 Nachos 硬盘 “DISK” 的创建及硬盘格式化（创建文件系统）的处理过程。
  
  4. 利用命令 hexdump -C DISK 查看硬盘格式化后硬盘的布局，理解格式化硬盘所完成的工作，以及文件系统管理涉及到的一些数据结构组织与使用，如头文件（FCB）、目录表与目录项、空闲块管理位示图等。
  
     结合输出结果，分析 FileSystem::FileSystem(..) 初始化文件系统时涉及到的几个模块，如 ../filesys/filehdr.h，directory.h(directory.cc)，../userprog/bitmap.h，理解文件头（FCB）的结构与组织、硬盘空闲块管理使用的位示图文件、目录表文件及目录下的组织与结构，以及它们在硬盘上的位置。
  
  5. 利用命令 nachos -cp ../test/small 复制文件 ../test/small 到硬盘 DISK 中。
  
  6. 利用命令 hexdump -C DISK 查看硬盘格式化后硬盘的布局，理解创建一个文件后相关的结构在硬盘上的存储布局。
  
  7. 复制更多的文件到 DISK 中，然后删除一个文件，利用 hexdump -C DISK 查看文件的布局，分析文件系统的管理策略。

（实验五）

Nachos 文件系统包括如下类模块，Disk、SynchDisk、BitMap、FileHeader、OpenFile、Directory、FileSystem，各类之间的相互关系如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202201728623.png" alt="image-20191202201728623" style="zoom:35%;" />

目前 Nachos 实现的文件系统存在诸多限制，其中之一是文件大小不能扩展，即无法在已经存在的文件尾部追加数据。因此实验五需要修改 Nachos 的文件系统，以满足如下功能。

1. 文件创建时，其大小可初始化为0。
2. 当一个文件写入更多的数据时，其大小可随之增大。
3. 要求能够在从一个文件的任何位置开始写入数据，即能够正确处理命令行参数 -ap，-hap 以及 -nap。





## 二、实验基本方法

#### 2.1 Nachos 的硬盘及文件系统

硬盘及文件系统具有以下特点：

+ 磁盘开始的4个字节是硬盘标识，其值为0x456789ab，用于指明该硬盘是一个 Nachos 硬盘。

  <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129161635607.png" alt="image-20191129161635607" style="zoom:57%;" />

+ 由于硬盘的前4个字节为硬盘标识，因此从第4～131号字节为0号扇区部分，第132～259号字节为1号扇区部分，即第4号字节为硬盘数据的0字节位。Nachos 文件系统具体的硬盘布局如下所示。

  <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129162901910.png" alt="image-20191129162901910" style="zoom:47%;" />

  可以发现0号扇区为位图文件头，1号扇区为文件目录表文件头。系统启动时需要根据目录文件文件头来访问根目录，因此将这两个重要的文件头存储到0、1号扇区中，便于系统启动时从明确、固定的位置来访问。

+ 硬盘的数据存储分为四级，分别是 硬盘 $\longrightarrow$ 磁道 $\longrightarrow$ 扇区 $\longrightarrow$ 字节。Nachos 硬盘中包括32个磁道，每个磁道包括32个扇区，每个扇区为128字节，每个字节8位，因此 Nachos 硬盘容量为 0x80KB，具体的代码可在 disk.h 找到，具体内容如下所示。

  <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129162028291.png" alt="image-20191129162028291" style="zoom:50%;" />

+ 由于一个扇区的内存为128字节，因此将每个逻辑块大小也设置为128字节与一个扇区对应，利用之后的编程实现。这也是和传统OS的一个区别，传统OS中的一个逻辑块一般包含 $2^n$ 个扇区，且 $n>0$。

+ Nachos 采用了一级目录结构，最多可创建10个文件。可在 ..filesys/filesys.cc 中宏定义处查看。

  <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191130151301018.png" alt="image-20191130151301018" style="zoom:40%;" />

+ 在 Nachos 中，一个目录文件由 “文件头+目录表” 组成。查看 ../filesys/directory.h 可以得到文件目录项的信息，其中信息如下所示。注意该类的三个成员变量为公有变量。除了文件头信息之外，下面还给出了文件目录表、文件头的成员变量信息。

  ```cpp
  class DirectoryEntry { // 文件目录
    public:
      bool inUse;				// 该目录项是否投入使用
      int sector;				// 文件头所在扇区号
      char name[FileNameMaxLen + 1];	// 文件名，+1用于'\0'
  };
  
  class Directory {	// 文件目录表
  	...
    private:
      int tableSize;			// 文件目录表大小
      DirectoryEntry *table;		// 文件目录表, 形式为 <file name, file header location>
  };
  
  class FileHeader { // 文件头
  	...
    private:
      int numBytes;			// 该文件的字节数
      int numSectors;			// 该文件的数据区块数
      int dataSectors[NumDirect];		// 每一块数据区所在扇区位置
  };
  ```

  由上述代码可知，每个文件头由 “文件头+数据块” 组成。在 Nachos 中，一个文件头的大小等于一个扇区大小，因此 $NumDirect = ((SectorSize - 2 * sizeof(int)) / sizeof(int)) = 30$，因此一个单文件最大为 $30*128B = 3840B$。除此之外，可以发现 Nachos 文件系统没有采用索引文件的方式分配文件数据，而是依次记录数据块所在扇区位置，类似于直接块的索引方式。

+ 查看 ../filesys/filesys.cc 中文件系统的构造函数，可以发现以下信息。

  ```cpp
  #define FreeMapSector 		0
  #define DirectorySector 	1
  #define FreeMapFileSize 	(NumSectors / BitsInByte)
  #define NumDirEntries 		10
  #define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)
  
  FileSystem::FileSystem(bool format)
  { 
      if (format) {   // 是否应该初始化磁盘
        BitMap *freeMap = new BitMap(NumSectors); // 创建文件位图
        Directory *directory = new Directory(NumDirEntries); // 创建包含10个文件目录项的文件目录表
        FileHeader *mapHdr = new FileHeader; // 创建文件位图的文件头
        FileHeader *dirHdr = new FileHeader; // 创建文件目录表的文件头
        
        // 第一步：文件位图中标记0、1号扇区被占用
        freeMap->Mark(FreeMapSector);		// 0号扇区为文件位图文件头
        freeMap->Mark(DirectorySector); // 1号扇区为文件目录表文件头
  
  			// 第二步：在文件系统中分配位图文件与文件目录表的空间（传入文件位图与空间大小）
        ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));   // 位图文件大小为128字节，1个扇区
        ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize)); // 十个文件目录项大小
  
  			// 第三步：将更新后的位图文件头、文件目录表文件头写入磁盘
        mapHdr->WriteBack(FreeMapSector);   // 传入对应文件头所在扇区号
        dirHdr->WriteBack(DirectorySector);
  
        // 第四步：创建位图文件、文件目录表的Openfile, Openfile中存储文件头与文件读写位置
        freeMapFile = new OpenFile(FreeMapSector);			// 传入文件头所在扇区, 用于创建Openfile文件头
        directoryFile = new OpenFile(DirectorySector);	// 文件读写位置初始为0
       
  			// 第五步：将位图文件信息、文件目录项信息传入对应Openfile中
        freeMap->WriteBack(freeMapFile);	 // 确定Openfile的起始扇区与结束扇区，开辟文件缓冲区
        directory->WriteBack(directoryFile); // 初始化整个文件
      } else {
        // 非初始化操作，则根据原有位图文件头、文件目录表文件头信息初始化Openfile
        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
      }
  }
  ```

  其中主要包含了位图文件、文件目录表的文件头、Openfile的创建，并将初始化信息写入磁盘。其中位图文件用每位的0、1来表示磁盘该位置是否空闲，分配时从前开始找，一旦有空位则直接分配。另外，从上面文件系统创建的部分可以发现，当文件创建后其大小则无法改变。

+ 至此可以得知 Nachos 文件系统在硬盘DISK中的布局如下表所示。

  <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202011522238.png" alt="image-20191202011522238" style="zoom:45%;" />



#### 2.2 Nachos 的文件系统命令

1. **nachos -d f** 表示打开 filesys 目录下的所有 debug 信息输出开关

2. **nachos [-d f] -f** 表示格式化 nachos 模拟的硬盘 DISK，在使用其它文件系统命令之前需要将硬盘格式化。硬盘格式化的内容已在上述部分说明清楚。该调试参数定义在 ../threads/system.cc 中。

   <img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202083934925.png" alt="image-20191202083934925" style="zoom:47%;" />

3. **nachos [-d f] -cp UNIX_filename nachos_filename**，将一个 Unix 文件系统中的文件 UNIX_filename 复制到 nachos 文件系统中，并重新命名为 nachos_filename。该参数及下述所有参数均定义在 ../threads/main.cc 中。

4. **nachos [-d f] -p nachos_filename** 表示输出 nachos 中文件 nachos_filename 的内容。
5. **nachos [-d f] -r nachos_filename** 表示删除 nachos 中文件 nachos_filename 。
6. **nachos [-d f] -l** 表示输出 nachos 当前的文件目录。
7. **nachos [-d f] -t** 表示测试 nachos 文件系统的性能。
8. **nachos [-d f] -D** 表示输出整个 nachos 文件系统的信息，包括位图文件、文件头、目录文件和普通文件。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202084506761.png" alt="image-20191202084506761" style="zoom:30%;" />

除此之外，还包括我们接下来需要自行实现的三个文件命令，-ap、-hap、-nap。

9. **nachos [-d f] -ap UNIX_filename nachos_filename** 表示将一个 UNIX 文件内容添加到 nachos 文件的末尾。
10. **nachos [-d f] -hap UNIX_filename nachos_filename** 表示将一个 UNIX 文件内容从 nachos 文件的中间部分开始向后添加并覆盖 nachos 文件的后半部分。
11. **nachos [-d f] -nap nachos_filename1 nachos_filename2** 表示将 nachos 中的 file1 中的内容添加到 file2 的文件末尾。



#### 2.3 两个 UNIX 命令

+ od 命令（od [OPTION] [FILE]）

该命令用于格式化输出文件中的数据，即对文件中的数据进行无二义性的解释。无论是IEEE754格式的浮点数还是ASCII码，od命令都能按照要求输出它们的值。

-a：表示ASCII码的名字

-b：选择单字节，并按照3个数值位对应的八进制数进行解释

-c：选择ASCII码字符或转义字符，用八进制显示文件偏移量。

-d：无符号2字节单位

-f：单精度浮点数

-i：十进制整型

-l：十进制长整型

-o：选择两个字节的单元，并按照八进制进行解释

-s：选择两字节单元并按照十进制解释

-x：选择两字节单元并按照十六进制解释

+ hexdump 命令（hexdump [OPTION] [FILE]）

使用 -c 参数，则简单输出 ASCII 字符信息，用十六进制显示文件偏移量。

使用 -C 参数，显示结果则分为三列（文件偏移量、字节的十六进制、ASCII字符）。

文件偏移量显示的单位不同，也是这两个命令一个区别。



#### 2.4 Nachos 文件系统的删除操作

Nachos 文件系统删除一个文件仅修改两个部分。

+ 一是将位图文件中被删除文件数据与文件头所在扇区清空；
+ 二是在文件目录表中将被删除文件对应的文件目录中的 inUse 变量清空，表明该目录项闲置。

下述代码为文件系统中的 remove 函数，函数中重要代码部分均有注释。

```cpp
/* 从目录表中移除、将位图中对应数据与头文件扇区清空 */
bool FileSystem::Remove(char *name) {
  	// 定义临时变量
    Directory *directory;
    BitMap *freeMap;
    FileHeader *fileHdr;
    int sector;
    
  	// 从 DISK 中取回文件目录表信息
    directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);
  	// 找到对应文件所在扇区
    sector = directory->Find(name);
    if (sector == -1) {
       delete directory;
       return FALSE;			 // 文件未找到 
    }
  	// 从DISK 中取回对应文件文件头的信息
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);
		// 从DISK 中取回位图文件
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);
		
    fileHdr->Deallocate(freeMap);  		// 将文件数据信息从位图中移除
    freeMap->Clear(sector);						// 将文件头信息从位图中移除
    directory->Remove(name);					// 文件目录表中移除该文件

    freeMap->WriteBack(freeMapFile);			// 更新位图
    directory->WriteBack(directoryFile);  // 更新文件目录表
    delete fileHdr;
    delete directory;
    delete freeMap;
    return TRUE;
} 

bool Directory::Remove(char *name) { 
    int i = FindIndex(name); 		//找到该文件在目录表中位置
    if (i == -1) return FALSE; 	// 文件不存在
    table[i].inUse = FALSE;			// 该目录项置空闲
    return TRUE;	
}
```



#### 2.5 DISK 文件的创建

nachos 硬盘文件的生成在初始化函数中实现，其具体过程分为以下几步。

1. ../threads/system.cc 中 Initialize 函数，创建 synchDisk 实例。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202195117156.png" alt="image-20191202195117156" style="zoom:47%;" />

2. SynchDisk 的初始化函数中创建了 disk 实例。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202195328434.png" alt="image-20191202195328434" style="zoom:30%;" />

3. Disk 的初始化函数中，下述代码生成了 DISK 文件。即当 DISK 文件不存在时，运行下述代码。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202195718470.png" alt="image-20191202195718470" style="zoom:50%;" />

4. ../machine/sysdep.cc 文件中的 OpenForWrite 函数生成了 DISK 文件。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202195632099.png" alt="image-20191202195632099" style="zoom:30%;" />



#### 2.6 Openfile::WriteAt() 函数

该函数用于将数据追加在 openfile 文件的某一个位置之后。主要细节在于一开始确认输入数据是否合法，然后确定数据所在扇区号，如果开头或结尾扇区不完整，则将缺少的数据拷贝到缓冲区中，最后将缓冲区中的数据写入 OpenFile 中即可。

```cpp
int OpenFile::WriteAt(char *from, int numBytes, int position) // 数据来源文件, 文件大小, 文件输入位置
{
    int fileLength = hdr->FileLength();     			// 返回文件字节数
    int i, firstSector, lastSector, numSectors;
    bool firstAligned, lastAligned;
    char *buf;
		// 检查输入是否合法以及避免输入数据超过文件大小
    if ((numBytes <= 0) || (position >= fileLength)) return 0;
  	if ((position + numBytes) > fileLength) numBytes = fileLength - position;

    firstSector = divRoundDown(position, SectorSize);   							// 确定第一个扇区
    lastSector = divRoundDown(position + numBytes - 1, SectorSize);   // 确定最后一个扇区
    numSectors = 1 + lastSector - firstSector;  											// 输入数据涵盖扇区数
    buf = new char[numSectors * SectorSize];    											// 创建完整的数据缓冲区		

  	// 确定起始位置是否为扇区开头，结束位置是否为扇区结尾
    firstAligned = (bool)(position == (firstSector * SectorSize));
    lastAligned = (bool)((position + numBytes) == ((lastSector + 1) * SectorSize));
  	// 如果起始位置不在扇区开头或结尾，则将开头和结尾扇区的全部内容放入缓冲区中
    if (!firstAligned)
        ReadAt(buf, SectorSize, firstSector * SectorSize);	
    if (!lastAligned && ((firstSector != lastSector) || firstAligned))
        ReadAt(&buf[(lastSector - firstSector) * SectorSize], 
				SectorSize, lastSector * SectorSize);	

    // 将 from 中的数据拷贝到缓冲区的对应区域中
  	bcopy(from, &buf[position - (firstSector * SectorSize)], numBytes);
		// 将缓冲区数据写入到 openfile 的对应扇区中
    for (i = firstSector; i <= lastSector; i++)	
        synchDisk->WriteSector(hdr->ByteToSector(i * SectorSize), 
				&buf[(i - firstSector) * SectorSize]);
    delete [] buf;
    return numBytes;
}
```





## 三、源代码及注释

在实验原理部分已经介绍了一些关键函数，该部分的主要内容是列出本次实验中自行设计的代码并附上关键注释。

#### 3.1 fstest.cc

> void Append(char *from, char *to, int half);

```cpp
void Append(char *from, char *to, int half) {
    FILE *fp;														// 关键变量
    OpenFile* openFile;
    int amountRead, fileLength, start;	// start为添加文件的开始位置
    char *buffer;
    // 打开UNIX文件
    if ((fp = fopen(from, "r")) == NULL) {	 
      printf("Copy: couldn't open input file %s\n", from);
      return;
    }
    // 计算UNIX文件长度
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);
		// 添加文件大小为0
    if (fileLength == 0) {
        printf("Append: nothing to append from file %s\n", from);
        return;
    }
	 	// 打开目标文件
    if ( (openFile = fileSystem->Open(to)) == NULL) {
    	if (!fileSystem->Create(to, 0)) { // 目标文件不存在，因此重新创建一个
    	    printf("Append: couldn't create the file %s to append\n", to); // 文件创建失败
    	    fclose(fp);
    	    return;
    	}
    	openFile = fileSystem->Open(to); // 打开新创建的文件
    }
    ASSERT(openFile != NULL);
    start = openFile->Length(); 			// 给start位置赋值
    if (half) start = start / 2;
    openFile->Seek(start);
    
    buffer = new char[TransferSize];	// 定义传输数据的缓冲区
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0) {
        int result = openFile->Write(buffer, amountRead);   // 会调用WriteAt函数
        if(result < 0){								// 数据读取发生错误
            printf("\nERROR!!!\n");
            printf("Insuficient Disk Space, or File is too big!\nWriting Terminated!\n");
            break;
        }
        ASSERT(result == amountRead);
    }
    delete [] buffer;
    // 将文件头写回硬盘
    openFile->WriteBack();
    DEBUG('f',"inodes have been written back\n");
    // 关闭UNIX与Nachos文件
    delete openFile;
    fclose(fp);
}
```

> void NAppend(char *from, char *to);

```cpp
void NAppend(char *from, char *to) {
    OpenFile* openFileFrom;							// 关键变量定义
    OpenFile* openFileTo;
    int amountRead, fileLength, start;	// start为appending的开始位置
    char *buffer;
  	// from文件不能与to文件相同
    if (!strncmp(from, to, FileNameMaxLen)) { 
    	printf("NAppend: should be different files\n");
    	return;
    }
		// from文件不存在
    if ( (openFileFrom = fileSystem->Open(from)) == NULL) {
    	printf("NAppend:  file %s does not exist\n", from);
    	return;
    }
		// from文件的长度
    fileLength = openFileFrom->Length();
    if (fileLength == 0) {  // 添加的数据为空
    	printf("NAppend: nothing to append from file %s\n", from);
    	return;
    }
	 	// 打开to文件
    if ( (openFileTo = fileSystem->Open(to)) == NULL) {
    	if (!fileSystem->Create(to, 0)) { // to文件不存在，则重新创建一个
    	    printf("Append: couldn't create the file %s to append\n", to);
    	    delete openFileFrom;
    	    return;
    	}
    	openFileTo = fileSystem->Open(to);
    }
    ASSERT(openFileTo != NULL);
    // 将to文件指针移动到末尾
    start = openFileTo->Length();
    openFileTo->Seek(start);
    // 将添加的数据通过数据缓冲区传送
    buffer = new char[TransferSize];
    // 将from文件指针移到文件开头
    openFileFrom->Seek(0);
    while ( (amountRead = openFileFrom->Read(buffer, TransferSize)) > 0) {
        int result = openFileTo->Write(buffer, amountRead);
        if(result < 0){
            printf("\nERROR!!!\n");	// 数据传输过程出现错误
            printf("Insuficient Disk Space, or File is Too Big!\nWriting Terminated!\n");
            break;
        }
        ASSERT(result == amountRead);
    }
    delete [] buffer;
    // 更新to文件头
    openFileTo->WriteBack();
    DEBUG('f',"inode have been written back!\n");
    // 关闭 to 和 from 文件
    delete openFileTo;
    delete openFileFrom;
}
```



#### 3.2 OpenFile

> class OpenFile{};

我们在 OpenFile 中添加了新的成员变量，hdrSector，用于表示文件头所在扇区号。

```cpp
class OpenFile {
  private:
  	FileHeader *hdr;							// 文件头句柄
    int seekPosition, hdrSector;	// 文件读取位置、文件头所在扇区号
};
```

因此我们需要该类的构造函数，赋予扇区号的初值。

```cpp
OpenFile::OpenFile(int sector) {
    hdrSector = sector; 					// 赋予初值
    hdr = new FileHeader;
    hdr->FetchFrom(sector);				// 获取文件头
    seekPosition = 0;
}
```

> int WriteAt(char *from, int numBytes, int position);

由于在第二部分已经给出了未修改部分的 WriteAt 函数，因此下面仅给出该函数修改的关键部分内容。

```cpp
if ((numBytes <= 0) || (position > fileLength)) return -1;			// 检查输入
if ((position + numBytes) > fileLength){
  int incrementBytes = (position+numBytes)-fileLength;
  BitMap *freeBitMap = fileSystem->getBitMap(); 								// 取出位图文件
  bool hdrRet;
  hdrRet = hdr->Allocate(freeBitMap,fileLength,incrementBytes); // 此处修改文件头、位图文件
  if(!hdrRet) return -1;  						// 分配空间时出错
  fileSystem->setBitMap(freeBitMap);  // 更新位图文件
}
```

> void WriteBack();

```cpp
void OpenFile::WriteBack(){
    hdr->WriteBack(hdrSector);
}
```



#### 3.3 FileSystem

> BitMap* getBitMap();

```cpp
BitMap* FileSystem::getBitMap() {
    BitMap *freeBitMap = new BitMap(NumSectors); //1024个扇区
    freeBitMap->FetchFrom(freeMapFile);
    return freeBitMap;
}
```

> void setBitMap(BitMap* freeMap);

```cpp
void FileSystem::setBitMap(BitMap* freeMap) {
    freeMap->WriteBack(freeMapFile);    // 将位图文件写回磁盘
}
```



#### 3.4 FileHeader

> bool Allocate(BitMap *freeMap, int fileSize, int incrementBytes);

```cpp
bool FileHeader::Allocate(BitMap *freeMap,int fileSize, int incrementBytes) {
    // 修改位图文件信息以及文件头的信息，但修改结果均未写入磁盘中
    if(numSectors > 30) return false;   								// 超出限定大小
    if((fileSize == 0) && (incrementBytes > 0)){ 				// 在空文件后追加数据
        if(freeMap->NumClear() < 1) return false; 			// 空间不足
        dataSectors[0] = freeMap->Find(); 							// 先分配一个空闲磁盘块，并更新文件头信息
        numSectors = 1;
        numBytes = 0;
    }
    numBytes = fileSize;
    int offset = numSectors * SectorSize - numBytes;		// 原文件最后一个扇区块空闲空间
    int newSectorBytes = incrementBytes-offset; 				// 需要填的数据-最后一个扇区块空闲空间
    // 最后一个扇区的空闲空间足够
    if(newSectorBytes <= 0){
        numBytes = numBytes+incrementBytes; 						// 更新文件头中的文件大小
        return true;
    }
    // 最后一个扇区的空闲空间不足
    int moreSectors = divRoundUp(newSectorBytes,SectorSize);
    if(numSectors+moreSectors > 30) return false;   		// 文件过大，超过30个磁盘块
    if(freeMap->NumClear() < moreSectors) return false; // 无足够扇区用于分配
    for(int i = numSectors; i < numSectors+moreSectors; i++) dataSectors[i] = freeMap->Find();
    numBytes = numBytes+incrementBytes;     						// 更新文件大小
    numSectors = numSectors+moreSectors;    						// 更新文件扇区块数
    return true;
}
```





## 四、实验测试方法及结果

#### 1. 测试文件

利用 UNIX 的命令 od 或 hexdump 来检查模拟硬盘 DISK 的内容。测试文件为 ../filesys/test 中 small、medium、big 三个文件。

使用 od -c test/small 来显示文件信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202092943278.png" alt="image-20191202092943278" style="zoom:47%;" />

使用 hexdump -c test/small 来显示文件信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202093018155.png" alt="image-20191202093018155" style="zoom:47%;" />

使用 hexdump -C test/small 来显示文件信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202093048410.png" alt="image-20191202093048410" style="zoom:43%;" />



#### 2. 编译Nachos的文件系统

编译 code/filesys，Makefile 内容如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129160209569.png" alt="image-20191129160209569" style="zoom:35%;" />

Makefile.local 文件内容如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129160921423.png" alt="image-20191129160921423" style="zoom:35%;" />



#### 3. 测试 Nachos 文件系统

（a）运行 nachos -f 命令，即创建一个 nachos 模拟硬盘 DISK 并创建一个文件系统，当前目录中出现了 DISK 文件。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202093723639.png" alt="image-20191202093723639" style="zoom:47%;" />

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094300565.png" alt="image-20191202094300565" style="zoom:50%;" />

（b）在上述命令基础上，运行 nachos -D，显示硬盘 DISK 中的文件系统，如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202093853382.png" alt="image-20191202093853382" style="zoom:30%;" />

上述信息表示0号扇区为位图文件头，1号扇区为文件目录表文件头，2号扇区为位图文件，3、4号扇区为文件目录表文件。

（c）继续运行 od -c DISK 命令，即用 od 命令显示 DISK 文件中信息，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094153866.png" alt="image-20191202094153866" style="zoom:33%;" />

（d）运行 hexdump -c DISK 命令，即用 hexdump 命令显示 DISK 文件中信息，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094430099.png" alt="image-20191202094430099" style="zoom:33%;" />

（e）运行 hexdump -C DISK 命令，同时显示 DISK 文件偏移量、字节的十六进制、ASCII字符 三个信息，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094555934.png" alt="image-20191202094555934" style="zoom:33%;" />

（f）运行 nachos -cp test/small small 命令，将 small 文件拷贝到 nachos 模拟硬盘中。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094749264.png" alt="image-20191202094749264" style="zoom:47%;" />

拷贝完成后，我们运行 nachos -l 命令，输出 nachos 当前的文件目录。我们可以查看到输出结果中包含了 small 这一刚拷贝进去的文件，即 cp 命令执行结果正确。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202094918929.png" alt="image-20191202094918929" style="zoom:33%;" />

继续运行 nachos -p small 命令，查看 nachos 模拟硬盘中 small 文件的具体信息。我们可以查看到输出结果中包含了 “This is the spring of our discontent.” 信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202095948141.png" alt="image-20191202095948141" style="zoom:33%;" />

再执行 nachos -D 命令，输出整个 nachos 文件系统的信息，具体信息如下所示。可以看到与最初的nachos模拟硬盘相比，复制small文件后，模拟硬盘中5、6扇区被占用，其中5号扇区为small文件的文件头，6号扇区为small文件的具体信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202101205659.png" alt="image-20191202101205659" style="zoom:25%;" />

继续执行 od -c DISK 命令，用 od 命令查看当前 DISK 文件中信息，输出结果如下所示。不难发现，DISK 中 “small“ 信息为3号扇区中文件目标表文件中的信息，而 ”This is the spring of our discontent.“ 则为6号扇区中 small 文件中的信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202101503181.png" alt="image-20191202101503181" style="zoom:30%;" />

再执行 hexdump -c DISK 命令，即用 hexdump 命令查看 DISK 文件中信息，输出结果如下所示。该输出信息与od -c 命令的最大区别在于该命令文件偏移量用16进制表示，而 od -c 命令的文件偏移量用8进制表示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202102159823.png" alt="image-20191202102159823" style="zoom:30%;" />

再执行 hexdump -C DISK 命令，输出结果如下所示。该命令与 hexdump -c 命令最大的差别在于该命令还显示了ASCII 字符的信息。也可以观察到下述文件的数据信息与上述操作显示的信息均一致。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202103954290.png" alt="image-20191202103954290" style="zoom:30%;" />

（h）接下来我们将 medium 文件拷贝进文件系统，然后继续执行上述的 DISK 信息输出，以此来查看模拟硬盘发生的变化。由于在下一部分 “nachos文件系统在硬盘上的布局“ 中较为明确地包含了文件布局的信息，因此接下来我们仅对 hexdump -C DISK 命令之后的结果进行简单分析。（详细分析在下一部分中已经包含的较为完善）

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202172931821.png" alt="image-20191202172931821" style="zoom:30%;" />

不难发现，文件目录表中包含了medium文件，且在之后的数据扇区中也出现了medium文件的内容。

（i）接下来我们将 big 文件拷贝进文件系统，然后继续执行上述的 DISK 信息输出，以此来查看模拟硬盘发生的变化。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191204002308593.png" alt="image-20191204002308593" style="zoom:37%;" />

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191204002419357.png" alt="image-20191204002419357" style="zoom:34%;" />

​																					  ......

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202173151783.png" alt="image-20191202173151783" style="zoom:30%;" />

​																					  ......

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202173215120.png" alt="image-20191202173215120" style="zoom:30%;" />

由上述输出信息可以发现文件目录表中包含了big文件，且之后的数据扇区中也出现了big文件的内容。

（j）接下来我们利用 ../nachos -r medium 命令将 medium 文件从 DISK 中删除，并利用 hexdump -C DISK 命令输出删除后的结果。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202173504519.png" alt="image-20191202173504519" style="zoom:25%;" />

可以发现文件目录中仍然包含 medium 文件名，且在数据块中也包含 medium 文件的数据内容。而删除操作的真正变化在于位图文件中 medium 头文件、数据文件所在扇区均被清空，且文件目录表中 medium 对应文件的 inUse 变量被清空。这些操作的代码细节在第二部分的实验基本方法中均已提及。



#### 4. 查看 Nachos 文件系统在硬盘上的布局

##### 4.1 硬盘格式化

格式化硬盘分别两步，首先（1）将原有 DISK 文件删除，（2）再利用 nachos -f 命令格式化硬盘。
接下来我们利用 hexdump -C DISK 命令查看 DISK 中最初的数据信息。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202110046212.png" alt="image-20191202110046212" style="zoom:30%;" />

+ 前4个字节的信息（0x0～0x3）

在上述数据信息中，一行显示16个字节信息，每个字节用两个16进制来表示，其中**前4个字节**（0x0～0x3）为 ab 89 67 45 与之前代码中宏定义的 MagicNumber 一致，作为该磁盘的标识。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191129161635607.png" alt="image-20191129161635607" style="zoom:57%;" />

+ 0号扇区信息（0x4～0x83）

之后128个字节（0x4～0x83）为**0号扇区**存储空间，存放了位图文件的文件头。文件头中变量如下所示，依次为文件字节数、区块数以及每一区块所在扇区位置。因此0号扇区中的第一个字节（0x4～0x7）表示位图文件字节数，即 0x80，为128字节。第二个字节（0x8～0xB）表示位图文件的扇区数，即 0x01，为1个扇区。因此第三个字节表示位图文件第1个数据块所在扇区位置，即 0x02，为2号扇区。

```cpp
class FileHeader { // 文件头
	...
  private:
    int numBytes;			// 该文件的字节数
    int numSectors;			// 该文件的数据区块数
    int dataSectors[NumDirect];		// 每一块数据区所在扇区位置
};
```

+ 1号扇区信息（0x84～0x103）

再看**1号扇区**中的信息（0x84～0x103），该扇区存放了目录表文件头数据，因此前4个字节为目录表文件大小，为0xc8，即200个字节。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202112145561.png" alt="image-20191202112145561" style="zoom:47%;" />

```cpp
class DirectoryEntry { // 文件目录
  public:
    bool inUse;				// 该目录项是否投入使用
    int sector;				// 文件头所在扇区号
    char name[FileNameMaxLen + 1];	// 文件名，+1用于'\0'
};
```

由于在宏定义中定义了文件名长度，因此我们可以得知一个文件目录项大小为一个bool类型，10个char类型，1个int类型。但是 c++ 中要采用数据对齐，因此不同变量存储空间以占用空间最大的变量类型为准，因此每个变量所占空间需要为4的倍数，因此一个文件目录项大小为 4+4+12 = 20 个字节。又由于我们最多定义10个目录项，因此目录表文件大小为200个字节。

1号扇区第5～8字节表示系统为目录文件数据所分配的扇区数，即0x02，数值为2，分配了两个扇区。

0x8C～0x8F表示第1个数据块所在扇区，为0x03，即3号扇区。

0x90～0x93表示第2个数据块所在扇区，为0x04，即4号扇区。

+ 2号扇区信息（0x104～0x183）

由位图文件头中的信息可知，2号扇区存储了位图文件的信息。位图文件第1个字节为0x1f，即表示前8个扇区是否被占用。11111000，即前5个扇区被分配，其余扇区均为空闲，与间接验证了之前数据的正确性。

+ 3号扇区（0x184～0x203）与4号扇区信息（0x204～0x283）

3、4号扇区存储了文件目录项的具体信息，目前没有任何文件，因此这两个扇区中值均为0。

最后 nachos -D 输出整个文件系统的信息用于验证上述观察结果。可以发现只有前5个扇区中有文件，与之前观察的数据一致。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202113853501.png" alt="image-20191202113853501" style="zoom:37%;" />



##### 4.2 复制 small 文件到硬盘

利用 nachos -cp test/small small 将 small 文件复制到硬盘中，并利用 hexdump -C DISK 命令来输出模拟磁盘数据。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202132705056.png" alt="image-20191202132705056" style="zoom:35%;" />

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202132730663.png" alt="image-20191202132730663" style="zoom:30%;" />

+ 0号扇区（0x4～0x83）与1号扇区（0x84～0x103）仍然存储位图、文件目录表的文件头，文件头信息没有变化，其文件中的数据发生了变化。

+ 2号扇区（0x104～0x183）存储位图文件，其中第一个字节为0x7f，其余字节均为0。0x7f二进制形式为 11111110，即前7个扇区均被占用，其余扇区仍为空。

+ 3号扇区（0x184～0x203）存储文件目录表的部分信息，其中每个目录项都是个三元组 `<bool inUSe, int sector, char name[FileNameMaxLen]+1>`，且由文件头信息可知当前只有一个目录项，因此前4个字节（0x184～0x187）表示三元组中的inUse，此处采用数据对齐的原则，即与最大的数据类型保持一致，因此此处bool变量占用了4个字节。第一个字节值为01，表示目录项正被使用。

  第5～8个字节（0x188～0x18B）表示该目录项文件头存储的扇区号，此处为05，即5号扇区。

  之后的10个字节为文件名，此处为ASCII码值为 small。

  由于当前只有1个目录项，因此其余目录项均为空。

+ 4号扇区（0x204～0x283）为目录表文件的第2个扇区，目前为空。

+ 5号扇区（0x284～0x303）为 “small” 文件的文件头。每个文件头存储一个3元组，`<int numBytes, int numSectors, int dataSectors[NumDirect]>`。

  第1～4个字节（0x284～0x287），表示该文件的字节数，为0x26，即38字节，查看small文件中的数据，恰好为38个字节，此处数据正确。

  第5～8个字节（0x288～0x28B），表示该文件的扇区数，为0x01，即1字节，该文件占用了1个扇区。

  第9～12个字节（0x28C～0x28F），表示该文件第一个数据块所存储的扇区，为0x06，即6号扇区。

+ 6号扇区（0x304～0x383）为 “small” 文件的数据块，其中 0x304～0x329 为该文件存储信息，0x0a为换行符。

此处还有一个地方需要注意。即文件目录表占用200个字节，其中并没有计算文件目录表中 tableSize 这个变量，即 nachos 并没有将这个变量存储在模拟 DISK 上，但这个变量仍然存在于 nachos 的可执行文件中，并不会凭空消失。由下面文件目录表的 WriteBack 函数可以得知，nachos 仅将目录表存储在了 DISK 上。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202150056262.png" alt="image-20191202150056262" style="zoom:37%;" />

总结一下，在仅包含一个small文件的文件系统中，模拟硬盘DISK的分布如下。

| 扇区起始地址 | 扇区号  |      扇区存储内容      |
| :----------: | :-----: | :--------------------: |
|  0x04～0x83  | 0号扇区 |       位图文件头       |
| 0x84～0x103  | 1号扇区 |    文件目录表文件头    |
| 0x104～0x183 | 2号扇区 |        位图文件        |
| 0x184～0x203 | 3号扇区 | 目录表文件第一个数据块 |
| 0x204～0x283 | 4号扇区 | 目录表文件第二个数据块 |
| 0x284～0x303 | 5号扇区 |      small文件头       |
| 0x304～0x383 | 6号扇区 |       small文件        |

最后输入命令 nachos -D，输出整个文件系统的信息用于数据验证。由下图输出信息可知上述数据分析过程正确。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202150909302.png" alt="image-20191202150909302" style="zoom:37%;" />



##### 4.3 复制 big 文件到硬盘

利用 nachos -cp test/big big 将 big 文件复制到硬盘中，并利用 hexdump -C DISK 命令来输出模拟磁盘数据。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202151730908.png" alt="image-20191202151730908" style="zoom:47%;" />

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202151815049.png" alt="image-20191202151815049" style="zoom:30%;" />

​																						......

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202151832438.png" alt="image-20191202151832438" style="zoom:30%;" />

+ 0号扇区（0x4～0x83）与1号扇区（0x84～0x103）仍然存储位图、文件目录表的文件头，文件头信息没有变化，其文件中的数据发生了变化。

+ 2号扇区（0x104～0x183）存储位图文件，其中第一个字节为0xff，第二个字节为0x1f，其余字节均为0。即1111111111111000，表示前13个扇区均被占用，其余扇区仍为空。

+ 3号扇区（0x184～0x203）中增加了big文件的目录项，表明big文件头存储在7号扇区。

+ 4、5、6号扇区没有发生变化，因此不再赘述。

+ 7号扇区（0x384～0x403）存储了big文件的文件头。

  第1～4个字节（0x384～0x387），big文件大小为0x260，即608个字节。

  第5～8个字节（0x388～0x38B），big文件占用扇区数为0x05，即5个扇区。

  之后的5个int数据表明了上述5个扇区分别存储在8、9、10、11、12号扇区中。

+ 8、9、10、11、12号扇区中存放big文件的数据。

总结一下，在包含 small 和 big 文件的文件系统中，模拟硬盘DISK的分布如下。

| 扇区起始地址 |   扇区号    |      扇区存储内容      |
| :----------: | :---------: | :--------------------: |
|  0x04～0x83  |   0号扇区   |       位图文件头       |
| 0x84～0x103  |   1号扇区   |    文件目录表文件头    |
| 0x104～0x183 |   2号扇区   |        位图文件        |
| 0x184～0x203 |   3号扇区   | 目录表文件第一个数据块 |
| 0x204～0x283 |   4号扇区   | 目录表文件第二个数据块 |
| 0x284～0x303 |   5号扇区   |      small文件头       |
| 0x304～0x383 |   6号扇区   |       small文件        |
| 0x384～0x403 |   7号扇区   |       big文件头        |
| 0x404～0x683 | 8～12号扇区 |        big文件         |

最后输入命令 nachos -D，输出整个文件系统的信息用于数据验证。由下图输出信息可知上述数据分析过程正确。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202170100449.png" alt="image-20191202170100449" style="zoom:30%;" />



##### 4.4 在硬盘上删除文件

利用 nachos -r small 将 small 文件从模拟硬盘中删除，并利用 hexdump -C DISK 命令来输出模拟磁盘数据。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202170340172.png" alt="image-20191202170340172" style="zoom:30%;" />

​																						......

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202170429059.png" alt="image-20191202170429059" style="zoom:30%;" />

+ 可以看到位图文件中数据发生了变化，从ff变成了9f，即1111变成了1001，5、6号扇区变空闲。
+ 目录表文件中small文件的inUse变量从0x01变成0x00，即标志该目录项空闲。
+ 除此之外，不难发现 small 文件在目录表中文件名、文件头所占扇区号均为清除。而且 small 文件头中的信息也均未被清楚，文件的内容也未被清楚。

输入命令 nachos -D，输出整个文件系统的信息用于数据验证。由下图输出信息可知上述数据分析过程正确。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202171148629.png" alt="image-20191202171148629" style="zoom:25%;" />

nachos 调用了 FileSystem::Remove(char *name) 删除文件，上述实验原理中已提及该部分内容。

最后，我们可以发现，如果被删除文件还没有被覆盖，则我们可以根据文件名在目录表中找到该文件对应的目录项，将 inUse 变量置1，再在位示图中将其对应数据、文件头扇区恢复即可。nachos 删除文件的策略为文件恢复带来了极大的便利。



#### 5. 扩展文件的实现与测试

##### 5.1 nachos -ap 与 -hap 命令的实现

我们观察 main.cc 中是如何实现 nachos -ap 命令的，具体代码如下所示。

```cpp
else if (!strcmp(*argv, "-ap")) {  // 将 UNIX 的文件添加到 Nachos 文件的尾部
  ASSERT(argc > 2);
  Append(*(argv + 1), *(argv + 2), 0);
  argCount = 3;
} 
```

可以发现 “-ap” 的命令调用了函数 Append()，而该函数主要调用的是 OpenFile::Write()，OpenFile::Write() 调用的是 OpenFile::WriteAt()，因此我们接下来考虑如何修改 OpenFile::WriteAt() 函数来实现该功能。

> 修改 OpenFile::WriteAt() 函数

由实验基本方法中对于原有 WriteAt 函数的分析可以看到，原有函数并不支持写入的数据超过其文件原有大小，因此我们现在需要修改该函数来实现可以从文件尾写数据的功能。

找到该函数中原有的两个约束，具体代码如下所示。

```cpp
// 检查输入是否合法以及避免输入数据超过文件大小
if ((numBytes <= 0) || (position > fileLength)) return 0;
if ((position + numBytes) > fileLength) numBytes = fileLength - position;
```

将第一个约束修改为返回 -1。而对于第二个约束我们需要分类讨论。

（1）如果原来文件最后一个扇区的剩余空间足以容纳要写入的 numBytes 个字节，则我们不需要为写入操作分配新的扇区，直接在原文件的最后一个扇区中写入数据即可。此处我们需要修改文件头中文件大小属性，并在文件写操作结束后将文件头写会硬盘原来的扇区中。

此种情况出现的原因在于文件的大小不一定是扇区的整数倍，但我们给文件分配的空间都是整数个扇区大小，因此文件所在的最后一个扇区可能会出现空间浪费，即最后一个扇区有空闲空间。

（2）如果原来文件最后一个扇区的剩余空间太小，无法容纳要写入的 numBytes 个字节，则需要为写入操作分配新的扇区，即将原文件的最后一个扇区写满后，将剩余数据写入新分配的扇区中。

因此我们修改第二个约束时，需要修改文件头中文件大小属性，并将新分配的扇区在空闲块管理位示图中对应位置置1，并将修改的文件头和位图写会到DISK中。修改的代码如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202221532084.png" alt="image-20191202221532084" style="zoom:50%;" />

> 修改 FileSystem 类，增加 setBitMap() 与 getBitMap() 函数

修改该类的目的是从能够从硬盘中读取空闲块位示图文件，并在位图文件的内容修改之后再写回磁盘中，修改后的代码如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202221909471.png" alt="image-20191202221909471" style="zoom:50%;" />

还需要在 FileSystem 类定义中声明这两个函数，具体声明如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191202222042241.png" alt="image-20191202222042241" style="zoom:60%;" />

> 修改 OpenFile::OpenFile() 以及 OpenFile::WriteBack() 函数

我们需要需要将修改后的文件头写回硬盘，因此我们需要修改上述两个函数。

下述代码为 OpenFile 的构造函数，查看该代码可以发现该类中文件头的句柄由文件头所在扇区号所决定。

```cpp
OpenFile::OpenFile(int sector) { 
    hdr = new FileHeader;
    hdr->FetchFrom(sector); // 取出该扇区中数据
    seekPosition = 0;   		// 文件读取位置为0
}
```

因此我们需要将修改后的文件头写回硬盘时，需要获取该文件头所在扇区号，因此我们在OpenFile类中增加一个 hdrSector 变量用于记录该文件头所在扇区号。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203113016811.png" alt="image-20191203113016811" style="zoom:100%;" />

修改 OpenFile 的构造函数，加入 hdrSector 变量的赋值。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203113628570.png" alt="image-20191203113628570" style="zoom:60%;" />

加入了 hdrSector 变量之后，我们再来实现 OpenFile::WriteBack() 函数，如下图所示。

![image-20191203113739473](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203113739473.png)

> 修改 FileHeader::Allocate() 函数

接下来我们需要实现将写入数据分配硬盘空间的函数。由于写入的数据可能利用文件最后一个扇区的剩余空间，也可能为其新分配扇区（硬盘块），因此在代码中我们需要分类讨论。

首先我们需要为 FileHeader 类添加构造函数，将文件头的扇区索引表清空，具体代码如下所示。

![image-20191203114337701](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203114337701.png)

然后我们再来实现 FileHeader::Allocate() 函数，实现过程如下所示。

我们重载 FileHeader::Allocate(BitMap *freeMap, int fileSize, int incrementBytes) 函数，并根据扩展的数据大小 incrementBytes 来判断是否需要分配新的扇区块，修改后的代码如下所示。

![image-20191203193958481](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203193958481.png)

在上述代码中，我们分为三步进行实现。

（1）我们判断是否可以分配，判断扇区是否足够，文件是否过大。如果是空文件我们需要先分配一个单独扇区。

（2）判断最后一个扇区中的剩余空间是否有足够空间存放，如果能放下就 不bi用开新空间。

（3）计算需要开辟的新扇区，并在位图文件中寻找新扇区进行分配。

> 修改 fstest.cc 的 Append() 函数

在 Append() 函数中，我们需要不断更新写指针的位置，并在写入操作结束后将文件头写回硬盘中。

![image-20191203141150320](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203141150320.png)

至此我们已经实现了 nachos 中 -ap 和 -hap 两个命令，接下来我们进行测试。

首先我们初始化磁盘，然后将 small 文件拷贝到 nachos 中，并输出 ./nachos -D 显示如下信息。

![image-20191203152507762](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203152507762.png)

可以发现 small 文件已经存储在了DISK上，接下来我们执行-ap命令，将big文件添加到small文件的后面，并输出 ./nachos -D 显示磁盘信息，具体结果如下所示。

![image-20191203152439784](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203152439784.png)

可以发现，big文件已经添加到了small文件的后面，结果正确。接下来我们继续测试-hap命令，我们先初始化磁盘，然后将small文件拷贝到DISK中，最后用-hap命令将big文件添加到small文件的后面，我们来查看输出结果有何不同。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203152710206.png" alt="image-20191203152710206" style="zoom:100%;" />

该命令即从nachos文件的中间位置开始添加，并覆盖掉nachos文件的后半部分，因此我们观察结果可知 small 文件的 `***end of file***` 结尾符也被覆盖了，由此可知 -hap 命令执行正确。

#####  

##### 5.2 nachos -nap 命令的实现

nachos -nap fromNachosFile toNachosFile。该命令将DISK中的其中一个Nachos文件添加到另一个Nachos文件的尾部。由于在实现-ap命令的过程中，我们已经实现了nachos文件的动态增长以及位图、文件头的更新等，因此我们只需在 NAppend() 函数中作出部分修改，并将最后的更新结果写会到磁盘中即可完成该命令的实现。具体修改内容如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203155238805.png" alt="image-20191203155238805" style="zoom:50%;" />

在该部分中，我们利用OpenFile中的Write函数不断通过缓冲区取出from文件中的数据然后写入到to文件中。并在写入完成之后将更新的文件头写入磁盘中。

接下来我们测试这个功能是否可以正常运行，我们先格式化DISK，然后依次利用-cp命令将small文件和big文件拷入DISK中，然后利用 ./nachos -D 命令查看DISK中的数据，结果如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203160043962.png" alt="image-20191203160043962" style="zoom:60%;" />

根据上述数据，可以看到small文件和big文件均已存储在了DISK中，接下来我们调用-nap命令，将small文件添加到big文件之后，然后再利用 ./nachos -D 命令输出执行结果，结果如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203160218316.png" alt="image-20191203160218316" style="zoom:67%;" />

可以看到big文件之后附上了small文件的内容，该命令运行正确。



##### 5.3 nachos 文件系统测试

（1）我们先格式化DISK，然后将small文件通过-cp命令拷入DISK中，并输出当前DISK中的信息，如下图所示。

![image-20191203161941551](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203161941551.png)

当前DISK中只有small一个文件。

（2）接下来我们尝试继续执行 ./nachos -cp test/small small 命令，观察输出结果。

![image-20191203162056058](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203162056058.png)

由于DISK中已有small文件，因此无法继续将small文件拷入DISK中。

（3）接下来我们执行 ./nachos -ap test/big small 命令，将 UNIX 中的 big 文件添加到 nachos 中的 small 文件之后，我们再查看 small 文件的信息。

![image-20191203162354501](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203162354501.png)

可以观察到位图信息的改变，以及 small 文件后面的确添加上了 big 文件，执行结果正确。

再执行 hexdump -C DISK，我们可以看到 -ap 命令的执行过程的确是将 small 文件所在扇区的剩余内容填满之后才开辟的新扇区继续添加数据。

![image-20191203162543769](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203162543769.png)

（4）执行 ./nachos -ap test/medium medium 命令，测试给一个空文件追加数据的功能。

![image-20191203162759815](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203162759815.png)

可以发现 medium 文件成功地添加进了 DISK 中，该命令执行正确。-ap 命令执行过程中，如果 to 文件不存在，则会创建一个 to 文件用于数据添加。

（5）再执行 ./nachos -ap test/big small 命令，查看 nachos 是否会为 small 文件开辟不连续的扇区。我们先执行 -p small 命令，查看 big 文件是否添加成功。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203163156642.png" alt="image-20191203163156642" style="zoom:67%;" />

由上图可知，big 文件添加成功。接下来我们再运行 hexdump -C DISK 命令查看 small 文件所分配的扇区是否不连续。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203163624733.png" alt="image-20191203163624733" style="zoom:67%;" />

由上图可知，small 文件的内容的确不连续，中间隔了medium文件头以及medium数据块的信息。

（6）执行 ./nachos -hap test/medium small 命令，测试从 small 中间写入文件的功能。执行完该命令后，我们再执行 hexdump -C DISK 命令来输出DISK中的具体信息，输出结果如下。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203163922145.png" alt="image-20191203163922145" style="zoom:60%;" />

可以发现该命令执行正确，medium文件信息成功从small文件的中间开始写入，并覆盖原有信息。而未被覆盖的信息保持不变。

（7）执行 ./nachos -nap medium small 操作，将 medium 文件添加到 small 文件之后，该命令执行后的输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203164344720.png" alt="image-20191203164344720" style="zoom:67%;" />

medium 文件的内容成功添加到了 small 文件之后，该命令执行结果正确。

（8）执行 ./nachos -r small 命令，测试文件删除功能。该命令执行后的DISK硬盘信息如下图所示。

![image-20191203173408494](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203173408494.png)

可以发现 DISK 中仅剩 medium 文件头与数据部分，分别占10、11、12号扇区。而 small 文件虽然被删除了，但由于 nachos 的删除机制，small 的数据并没有消失，只是位图文件中 small 文件所在扇区被清空了，以及 small 文件头的 inUse 变量被置 0 了。我们可以用 hexdump -C DISK 命令来验证这一观点。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203173622180.png" alt="image-20191203173622180" style="zoom:67%;" />

可以发现，small 文件的数据仍然存在，因此这一删除机制也更利于数据恢复。

（9）删除 small 文件之后，我们再执行 nachos -l、-p small 命令，输出结果如下图所示。

![image-20191203173805101](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203173805101.png)

再执行 ./nachos -p small 命令，输出结果如下所示。

![image-20191203173833913](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203173833913.png)

可以看到 small 文件的确已经从 nachos 中删除了，-r 命令执行结果正确。

（10）继续执行 nachos -ap test/big small 操作，查看 nachos 文件系统对一个文件长度的限制。由于在 nachos 中一个文件头所占空间为一个扇区，因此一个文件最多占用 30 个扇区，即为 $30*128B = 3840B$。

我们反复执行 nachos -ap test/big small 操作之后，当 small 文件超过最大容量限制时，将出现错误。

![image-20191203174532066](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203174532066.png)

因此我们利用 ./nachos -D 操作查看 DISK 中的信息，输出结果如下图所示。

![image-20191203174610160](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203174610160.png)

可以发现 small 文件占用了 30 个扇区，但30个扇区是否都占满了呢？我们执行 nachos -ap test/medium medium 命令，再执行 hexdump -C DISK 命令查看 DISK 中文件存储信息。

![image-20191203194528874](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203194528874.png)

由上图可以发现，30个扇区恰好完全填完。由下述的代码可以发现 nachos 执行 Append 操作时，每次从源文件中取出 10 字节数据存入 nachos 中，如果发现数据存入发生错误则退出。因此由于文件过大无法存入 DISK 时，未填充的字节空间一定小于 10 字节。

![image-20191203194852748](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203194852748.png)

（11）接下来我们继续执行 nachos -ap 操作来创建新的文件，测试 nachos 文件系统中是否最多只能创建 10 个文件。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203195210849.png" alt="image-20191203195210849" style="zoom:50%;" />

当创建到第 11 个文件时发生了错误。我们使用命令 ./nachos -l 来查看当前硬盘中的文件目录。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203195313785.png" alt="image-20191203195313785" style="zoom:30%;" />

可以发现当前 DISK 中恰好有 10 个文件，验证了 nachos 文件系统中最多只能创建 10 个文件说法的正确性。

（12）我们测试能否创建空文件。建立空文件 empty，使用命令 ./nachos -cp test/empty empty，查看是否可以创建成功。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203200835853.png" alt="image-20191203200835853" style="zoom:50%;" />

执行过程未发生错误，我们调用命令 ./nachos -l 来查看 DISK 中存在的文件，查看 empty 文件是否创建成功，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203201013174.png" alt="image-20191203201013174" style="zoom:55%;" />

由上述的输出结果可以得知 empty 文件创建成功，我们再调用命令 ./nachos -D 来查看 empty 文件是否真的存在，以及该文件所占用的空间，输出结果如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203201226117.png" alt="image-20191203201226117" style="zoom:50%;" />

由上述信息可以得知 empty 文件大小为 0，即空文件。因此在 nachos 中创建空文件的功能实现正确。

至此我们测试了本实验中给出的以及要求自行实现的各个命令行参数，由输出数据可知各命令行参数实现正确。





## 五、实验体会

1. nachos 实现的模拟硬盘 DISK，将所有数据全部存储在了 DISK 中，这意味着没有其它文件数据被存放于 nachos 的可执行文件中。包括位图、文件头等信息均存放于 DISK 中，使用时则从 DISK 中取出，用完即将指针回收。文件系统中的所有数据信息都采用句柄的形式，仅保存类指针，并不保存实际数据，使得空间利用效率大大提高。

2. 本次实验熟悉了 nachos 文件系统中的文件系统命令，包括将文件直接拷贝到 nachos 模拟硬盘中、输出模拟硬盘中的文件目录、输出模拟硬盘中某个文件的内容以及完整输出整个文件系统的所有信息等功能，功能较为全面且实用。

3. 除了 nachos 的文件系统命令外，该实验也介绍了 Linux 指令来查看文件中的各位数据，如 `od -c filename`、`hexdump -c filename`、`hexdump -C filename` 等指令来查看文件的 ASCII 码或 16 进制数据。

4. nachos 文件系统删除文件的策略比较经典且实用。删除文件时仅将文件目录表中该文件的 inUse 变量置 0，以及在位图中将该文件数据、文件头所在扇区清空，而不删除该文件的具体数据。这样的删除操作有几个好处，（1）不会影响删除操作的正确实现；（2）删除操作执行迅速；（3）便于数据恢复。

5. 在实验五中要求我们实现 -ap、-hap、-nap 三个命令，其中 -ap、-hap 命令调用的是 Append() 函数，而 -nap 命令调用的是 NAppend() 函数，其中三个命令的实现路径如下图所示。

   ![image-20191203225458352](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191203225458352.png)实现这三个命令的过程中，我们主要实现了 FileHeader::Allocate() 函数，其余函数更多的主要是在原基础上进行了一些修改或者实现比较简单。

6. 在 nachos 文件系统中，复制一个文件时所采用的策略是将文件按 10 字节大小的缓冲区进行划分，每次仅传输 10 字节大小的数据写入 DISK 中。这样做的目的是将一个大文件拆成多个小文件进行依次传输，降低传输过程中出错的概率，并且一旦数据传输出现问题，可以查看调试信息确定出错的位置。
7. 除此之外，在 nachos 文件系统中，将 A 文件 append 到 B 文件末尾时，如果 B 文件所占空间已经到达 30 个扇区，且最后一个扇区仍有空余。则 nachos 会将 A 文件按照最大 10 字节的方式进行拆分，边拆分边将数据传入 DISK 中，上一次数据传入成功，才会进行下一次传送。因此可能会出现 A 文件的部分内容传入了 B 文件末尾，但由于传输过程 B 文件所占扇区数已经达到了 30，导致传输中断。此时 B 文件最后一个扇区的空闲空间小于 10 个字节，且 A 文件仅传输了部分数据到 B 文件中。
8. 此次实验作为目前为止第一个修改了很多代码的一个实验。在修改代码的过程中，先分析需要实现的命令，再考虑有哪些已经实现的函数可以调用也显得十分重要。因此在之后的实验中，需要实验新的命令时，我们需要仔细分析实现这个命令的主要难点与困难点，并有效地利用实验中已经实现好的各个函数，使整个代码的冗余部分尽可能地少。
9. 在做实验的过程中，发现 nachos 的文件系统还有很多的扩展空间，比如我们可以一个从文件任意位置进行 append 的函数，甚至将 nachos 中的单级索引目录表修改成一个支持多级索引目录表的文件系统。除此之外，位图也有很大的优化空间。在 nachos 中，位图这个数据结构询问还有多少个空闲扇区以及查询一个空闲扇区时均采用了 O(n) 遍历的方式，但由于 nachos 文件系统中磁盘空间本身比较小，这种 O(n) 的遍历方式便不会有很明显的效率降低。但一旦 nachos 的磁盘空间比较大，这种方式的开销就非常巨大，更常用的应该是记录用一种类似于二叉树的结构将寻找空闲扇区的复杂度降为 O(logn)，记录一个变量用于维护当前还有多少个空闲扇区，从 O(n) 降到 O(1)，这种数据索引方式在较大的磁盘空间中使用更为有效。

10. 总结一下，此次实验介绍了 nachos 中的文件系统，并引导实验者实现了 3 个文件系统的命令行参数，加深了实验者对于 nachos 中文件系统的理解，并开始自行实现略为复杂的代码，为之后的实验打下了良好基础。