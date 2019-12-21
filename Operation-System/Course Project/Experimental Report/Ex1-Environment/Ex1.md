## 一、实验基础信息

##### 个人信息

201700130011 — 刘建东 — 17级菁英班

##### 实验信息

日期：2019.11.18

题目：$\text{NACHOS}$ 系统的安装与调试（实验一）

##### 实验任务

1. 安装编译 $Nachos$ 系统，理解 $Nachos$ 系统的组织结构，熟悉 C++ 编程语言
2. 安装测试 $gcc$ $\text{MIPS}$ 交叉编译器
   3. 掌握利用 $Linux$ 调试工具 $\text{GDB}$ 调试跟踪 $Nachos$ 的执行过程
   4. 通过跟踪 $Nachos$ 的 C++ 程序及汇编代码，理解 $Nachos$ 中线程上下文切换的过程
   5. 阅读 $Nachos$ 的相关源代码，理解 $Nachos$ 内核的工作原理及其测试过程





## 二、实验基本方法

此实验主要围绕着 $gdb$ 调试来测试 $Nachos$ 的具体执行过程。并根据 $Nachos$ 中的一些源代码来分析运行结果，进一步地理解 $Nachos$ 执行中的一系列细节问题。

##### 常见 $gdb $ 调试命令

> 开启调试

gdb ./nachos

> 查看地址

p currentThread（查看主线程对象的地址）

> 断点

1. 断点设置
   + b function（显示断点号及函数地址）
   + b num（List之后在具体行数上设置断点）
   + b *SWITCH+44（设置在SWITCH函数汇编代码的第44行）
2. 断点查看：info b
3. 断点清除
   + delete num（num为断点编号）
   + d（清除所有断点）

> 查看信息

1. 查看函数汇编代码：disass function
2. 查看函数源代码：list
3. 查看所有寄存器值：info r

> 程序运行

1. 从头开始运行：run
2. 继续运行：c
3. 单步运行：ni/si

![image-20191119143758712](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119143758712.png)

除了上述的 $gdb$ 调试方法之外，实验一中还涉及了一些汇编代码内容，实验过程中也是通过这些汇编代码来进一步分析 $Nachos$ 执行过程中的一系列细节内容。

##### 涉及的汇编内容

![image-20191119144032719](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119144032719.png)





## 三、源代码及注释

实验一中并不需要自己来实现代码，因此此处仅列出一部分与该实验相关的源代码。

```cpp
// main.cc 中部分内容
...
int main(int argc, char **argv)
{
    int argCount;										// 用于执行特定的指令所设置的变量
    DEBUG('t', "Entering main");		// 用于调试
    (void) Initialize(argc, argv);	// 初始化
    
#ifdef THREADS
    ThreadTest();
#if 0
    SynchTest();
#endif
...
  return(0);
}
```





## 四、实验测试方法及结果

#### 1. 安装 Nachos 系统

根据OS课程设计指南上的内容逐步完成即可，此处选择的是使用命令行进行安装。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119144914775.png" alt="image-20191119144914775" style="zoom:67%;" />



#### 2. gcc MIPS 交叉编译器的安装与测试

##### （1）安装 gcc MIPS 交叉编译器

此处仍然使用命令行进行安装，由 su root 命令打开权限，然后执行下述命令将交叉编译器安装到 /usr/local 目录中去。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119145217842.png" alt="image-20191119145217842" style="zoom:67%;" />

##### （2）测试 gcc MIPS 交叉编译器

执行步骤为删除 ../test/arch/unknown-i386-linux 中 depends 和 objects 目录中的所有文件，再删除 ../test 目录下的所有扩展名为 .noff 的文件，最后运行 make 即可。

> 删除扩展名为 .noff 文件的方法

1. find 目录 -name “*.abc”|xargs rm

2. 该命令有一定风险，可以先执行前半段，看看是不是要删除的文件, 然后再整条执行。

##### （3）为什么 nachos-3.4.tar.gz 一定要安装在 /usr/local 目录中？

查看 code/Makefile.dep 中 GCCDIR 变量的值，可以发现该编译器的路径即为 /usr/local/..

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119145821824.png" alt="image-20191119145821824" style="zoom:67%;" />



#### 3. 测试 Nachos

##### （1）在 code/threads 下执行 make 命令

如下图可以看到 make 命令执行成功。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119145956751.png" alt="image-20191119145956751" style="zoom:67%;" />

##### （2）运行 Nachos

在该目录下执行 ./nachos 命令，输出结果如下图所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119150114922.png" alt="image-20191119150114922" style="zoom:67%;" />

##### （2）初始化 Nachos 的设备与内核

此处调用了 Initialize 函数，其主要作用为创建 main 线程并且打开中断。 可以发现 main 线程创建后，后续的代码继续在主线程中执行。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119150419834.png" alt="image-20191119150419834" style="zoom:67%;" />

##### （3）测试 SynchTest()

运行 SynchTest() 可以看到如下的输出结果，是一个车过桥的同步测试问题。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119150718952.png" alt="image-20191119150718952" style="zoom:67%;" />



#### 4. 测试其它模块的功能

运行 code/monitor，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119150940720.png" alt="image-20191119150940720" style="zoom:37%;" />

运行 code/filesys，输出结果如下所示。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119151030633.png" alt="image-20191119151030633" style="zoom: 50%;" />  



#### 5. 利用 gbd 调试 Nachos C++ 代码的过程与方法

此处介绍了一些基础的调试的方法，我们在第二部分的实验基本方法中已经介绍过了，因此此处不再赘述。



#### 6. Nachos 的上下文切换

##### （1）查看对应文件的 c++ 源码

gbd 调试中的 list 操作。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119151236325.png" alt="image-20191119151236325" style="zoom:50%;" />

##### （2）查看给定函数的地址

通过 gbd 调试中的断点设置，即可查看下述 4 个函数的返回地址。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119151319629.png" alt="image-20191119151319629" style="zoom:67%;" />

##### （3）查看给定线程的地址

给定的线程：the main thread of the Nachos、the forked thread created by the main thread

我们找到 Simplethread() 函数，并查看其对应的汇编代码。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119152056269.png" alt="image-20191119152056269" style="zoom:40%;" />

根据该汇编代码，我们首先可以发现在（2）步中我们查看的 SimpleThread() 地址为 0x804a49b，而在该地址之前，仍然有三条汇编指令，这说明在 gdb 中，并不认为前 3 条指令属于该函数主体，因此函数入口才在第三条指令处。

上述汇编代码的分析过程是为之后的代码分析进行的铺垫，下面我们继续处理给定线程返回值的问题。

我们可以发现在 ../threads/system.cc 中的 Initialize() 函数中，语句 currentThread = new Thread("main") 创建了 Nachos 的主线程 “main”，并通过 current->setStatus(RUNNING) 将其状态设为就绪。因此要查看主线程的地址，我们需要在 currentThread->setStatus(RUNNING) 上设置断点，在该断点暂停后，再通过 p currentThread 查看主线程地址。

如下图所示，即可查看到 the main thread of the Nachos 的地址。

![image-20191119152944917](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119152944917.png)

下图即为 the forked thread created by the main thread 的地址。

![image-20191119153056555](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119153056555.png)

##### （4）当主线程第一次运行 SWITCH() 函数，执行到 SWITCH() 的最后一条指令 ret 时，CPU 返回的地址是多少？该地址对应程序的什么位置？

##### （5）当调用 Fork() 新建的线程首次运行 SWITCH() 函数时，当执行到函数 SWITCH() 的最后一条指令 ret 时，CPU 返回的地址是多少？该地址对应程序的什么位置？

最后两个问题分别查看的是第一次和第二次运行 SWITCH() 函数结束时的返回地址，其查看方法没有太大的区别，因此我们对于两个问题进行统一处理。

查看SWITCH()的返回地址一共有两种方法。

> 方法1

分析 ../threads/switch-linux.s 中 SWITCH() 的汇编代码，可以发现执行完0x0804bb69 <+5> movl 4(%esp),%eax­­­ 指令之后，寄存器 eax 中内容即为原线程（t1，oldThread）的地址。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119154350834.png" alt="image-20191119154350834" style="zoom:37%;" />

执行完指令 0x0804bb90 <+44> movl 8(%esp),%eax 后，寄存器 eax 中的内容是 新线程（t2，newThread）的地址。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119173020195.png" alt="image-20191119173020195" style="zoom:67%;" />

执行完指令 0x0804bbb1 <+77> movl _PC(%eax),%eax 后，寄存器 eax 中的内容 就是 SWITCH()的返回地址。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119173334102.png" alt="image-20191119173334102" style="zoom:67%;" />

可以发现，该地址对应于ThreadRoot()的第一条返回地址。

<img src="/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119173407070.png" alt="image-20191119173407070" style="zoom:67%;" />



> 方法2

第一种方法是根据汇编代码查看寄存器中的值来获得函数返回地址，这种方法具有一定的操作难度，因此我们采用一种更简单的方法，即直接单步调试查看返回地址。

我们用方法1找到了第一次SWITCH函数结束时的返回值，而对于第二次SWITCH函数结束返回值的查找我们用第二种方法来完成。

我们在SWITCH函数的ret位置设置断点，等程序下一次在此处停止时，进行单步调试，即可找到该函数结束时的返回地址。

![image-20191119174034778](/Users/gene_liu/Library/Application Support/typora-user-images/image-20191119174034778.png)

如上图所示，我们可以发现程序进入了Scheduler::Run()函数中，即第二次SWITCH函数结束的返回地址Scheduler::Run()函数中，且按该操作继续执行下去可以发现之后的返回地址都在Scheduler::Run()函数中。

而出现这种现象的原因在于ThreadRoot()是所有利用Thread::Fork()创建的线程的入口，因此首先第一次SWITCH函数结束的返回地址在ThreadRoot()中，而在子线程开始执行后，后续与主函数发生的上下文切换都是从上次被中断的地方开始执行，即Scheduler::Run()中语句SWITCH(oldThread,nextThread)之后，因此之后SWITCH函数结束后的返回地址都在Scheduler::Run()中。





## 五、实验体会

1. 此次实验主要是对于 gbd 调试方法的一个掌握，对于 gbd 中断点设置、查看寄存器、运行操作、查看源码/汇编代码的一个熟练运用。

   在实验过程中也根据 gbd 工具来实现了一系列操作，主要是查看各个函数的地址以及SWITCH函数两次结束后的返回地址，也进一步地加强了 gbd 使用的熟练性。

2. 除了 gbd 之外，本次实验中也正式引入了汇编代码。OS课程设计指南中也花了一定的篇幅介绍了 Nachos 中汇编代码的栈顶、栈底寄存器以及一些汇编中的操作。不过较之前面对于 gbd 的介绍，汇编的内容还是太多、太广泛了，实验中只是一个大致引入，想必在之后的实验里还会有更多大量的涉及。

3. 其次是对于 ubuntu 的一些命令的熟悉，如make、find 目录 -name "*.abc" | xargs rm 等命令，有利于之后的实验效率的一个提高。

4. 除了上述一些工具熟练度上的收获之外，此次实验作为第一个实验也正式开启了读源码之旅，每当遇到不明白的地方或者有一定困惑的地方都会打开对应的源代码或者汇编代码找到对应位置，或根据源码或根据寄存器内信息来对程序进行更进一步的理解。

   我想这种查看源码、汇编代码、gbd 调试查看寄存器内容的方法也是该课设想要培养的能力之一。最后总结一下，实验一作为第一个实验让我大致了解了该课设的一些实验工具以及课设研究方法，对整个课设的学习起到了铺垫的作用。



