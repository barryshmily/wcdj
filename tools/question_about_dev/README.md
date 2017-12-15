
# 一些问题记录
[TOC]

-----------------

## 计算机基础

1. 字符编码 ASCII，Unicode 和 UTF-8

http://www.ruanyifeng.com/blog/2007/10/ascii_unicode_and_utf-8.html

http://www.unicode.org/
https://www.joelonsoftware.com/2003/10/08/the-absolute-minimum-every-software-developer-absolutely-positively-must-know-about-unicode-and-character-sets-no-excuses/


## 编程语言部分

### C++

1. Function Object
如何实现下面的转换
``` cpp
// operator()
std::string operator()() const
{
	return std::to_string(a);
}

myclass obj;
std::string str = obj(); 

```
2. 函数是否占用对象的空间

3. 什么情况下迭代器会失效

4. 构造函数的初始值什么情况下必不可少？成员初始化顺序和什么有关？
(1) 如果成员是const，引用，或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始值列表为这些成员提供初值。
(2) 构造函数初始值列表只说明用于初始化成员的值，而不限定初始化的具体执行顺序。成员的初始化顺序与它们在类定义中的出现顺序一致：第一个成员先被初始化，然后第二个，以此类推。构造函数初始值列表中初始值的前后位置关系不会影响实际的初始化顺序。
解决方法：最好构造函数初始值的顺序与成员声明的顺序保持一致，而且如果可能的话，尽量避免使用某些成员初始化其他成员。

5. 隐士的类类型转换，如何抑制构造函数定义的隐士转换？为什么可以将const char *赋值给string对象？
因为接受一个单参数的const char *的string构造函数不是explicit的。
通过将构造函数声明为explicit来阻止隐士转换。其中，
(1) 关键字explicit只对一个实参的构造函数有效；
(2) 需要多个实参的构造函数不能用于执行隐士转换，所以无须将这些构造函数指定为explicit的；
(3) 只能在类内声明构造函数时使用explicit关键字，在类外部定义时不应重复；

6. extern "C" 的作用
http://blog.csdn.net/delphiwcdj/article/details/7173387

7. 使用表达式new失败如何处理？
默认情况下，如果new不能分配所要求的内存空间，它会抛出一个类型为bad_alloc的异常。我们可以改变使用new的方式来阻止它抛出异常：
int *p1 = new int;           // 如果分配失败，new抛出std::bad_alloc
int *p2 = new (nothrow) int; // 如果分配失败，new返回一个空指针
bad_alloc和nothrow都定义在头文件new中。

8. 拷贝构造函数时什么？什么时候使用它？
如果构造函数的第一个参数是自身类类型的引用，且所有其他参数（如果有的话）都有默认值，则此构造函数是拷贝构造函数。拷贝构造函数在以下几种情况下会被使用：
(1) 拷贝初始化(用＝定义变量)；
(2) 将一个对象作为实参传递给非引用类型的行参；
(3) 一个返回类型为非引用类型的函数返回一个对象；
(4) 用花括号列表初始化一个数组中的元素或一个聚合类中的成员；
(5) 初始化标准库容器或调用其insert/push操作时，容器会对其元素进行拷贝初始化； 

9. 拷贝赋值运算符是什么？什么时候使用它？合成拷贝赋值运算符完成什么工作？什么时候会生成合成拷贝赋值运算符？
(1) 拷贝赋值运算符本身是一个重载的赋值运算符，定义为类的成员函数，左侧运算对象绑定到隐含的this参数，而右侧运算对象是所属类类型的，作为函数的参数，函数返回指向其左侧运算对象的引用。
(2) 当对类对象进行赋值时，会使用拷贝赋值运算符。
(3) 通常情况下，合成拷贝赋值运算符会将右侧对象的非static成员逐个赋予左侧对象的对应成员，这些赋值操作时由成员类型的拷贝赋值运算符来完成的。
(4) 若一个类未定义自己的拷贝赋值运算符，编译器就会为其合成拷贝赋值运算符，完成赋值操作，但对于某些类，还会起到禁止该类型对象赋值的效果。

10. 重载运算符
重载运算符是具有特殊名字的函数，由关键字operator和其后要定义的运算符号共同组成。和其他函数一样，重载的运算符也包含返回类型，参数列表以及函数体。
对于一个运算符函数来说，它或者是类的成员，或者至少含有一个类类型的参数。例如，不能为int重新定义内置的运算符。
我们只能重载已有的运算符，而无权发明新的运算符号。
不能被重载的运算符：
:: .* . ?:

11. 虚函数的优缺点

12. 深拷贝和浅拷贝的区别

### C

1. 无符号和有符号是否可以比较
不可以，如果有符号的是负数，将类型提升为一个很大的无符号数

2. 可以定义一个引用的指针吗
不可以，引用不是对象。

3. 堆和栈的区别，在进程虚拟地址空间的位置

4. 指向常量的指针，和常量指针分别如何定义
const int * p1 = 1;
int i;
int * const p2 = &i;

5. 知道一个递增的区间段，如何计算这个区间段的中位数
比如，3,4,5,6,7
则中位数为：
方法1：(3 + 7) / 2 = 5 溢出问题
方法2：3 + (7 - 3) / 2 = 5 如果是迭代器只能这样用，因为迭代器没有定义加法，只定义了减法

6. 宏

测试方法：
``` 
gcc –E  macro.test.c
```

https://gcc.gnu.org/onlinedocs/cpp/Macros.html

### GCC

1. __attribute__ ((packed))的作用

告诉编译器取消结构在编译过程中的优化对齐按照实际占用字节数进行对齐，是GCC特有的语法。这个功能是跟操作系统没关系，跟编译器有关，gcc编译器不是紧凑模式的。

``` cpp
struct stCoRoutineAttr_t
{
    int stack_size;
    stShareStack_t*  share_stack;
    stCoRoutineAttr_t()
    {
        stack_size = 128 * 1024;
        share_stack = NULL;
    }
}__attribute__ ((packed));
```

2. __thread

线程局部存储(Thread Local Storage)是一种机制，通过这一机制分配的变量，每个当前线程有一个该变量的实例。在多线程编程中，如果使用__thread关键字修饰global变量，可以使得这个变量在每个线程都私有一份。

__thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。__thread变量每一个线程有一份独立实体，各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。

 __thread使用规则：只能修饰POD类型，不能修饰class类型，因为无法自动调用构造函数和析构函数。可以用于修饰全局变量，函数内的静态变量，不能修饰函数的局部变量或者class的普通成员变量。

 http://www.jianshu.com/p/13ebab5cd5b2

3. __sync_fetch_and_add

``` cpp
volatile unsigned int g_var = 0;

int atom_op()
{
    unsigned int old_var = __sync_fetch_and_add(&g_var, 1);
    printf("%d\n", old_var);

    //unsigned int new_var = __sync_add_and_fetch(&g_var, 1);
    //printf("%d\n", new_var);

    return 0;
}
```

4. 条件编译

``` cpp
static pid_t GetPid()
{
    static __thread pid_t pid = 0;
    static __thread pid_t tid = 0;
    if( !pid || !tid || pid != getpid() )
    {
        pid = getpid();
#if defined( __APPLE__ )
        tid = syscall( SYS_gettid );
        if( -1 == (long)tid )
        {
            tid = pid;
        }
#elif defined( __FreeBSD__ )
        syscall(SYS_thr_self, &tid);
        if( tid < 0 )
        {
            tid = pid;
        }
#else 
        tid = syscall( __NR_gettid );
#endif

    }
    return tid;
}

// or
static unsigned long long GetTickMS()
{
#if defined( __LIBCO_RDTSCP__) 
    static uint32_t khz = getCpuKhz();
    return counter() / khz;
#else
    struct timeval now = { 0 };
    gettimeofday( &now,NULL );
    unsigned long long u = now.tv_sec;
    u *= 1000;
    u += now.tv_usec / 1000;
    return u;
#endif
}
```


-----------------
## 网络部分

1. IP首部和TCP首部分别是多少个字节
20字节，8字节

2. tcp为什么要四次挥手
全双工的协议

3. 非阻塞网络编程有哪些方法，select和epoll的区别

4. 大小端的定义和判别方法

5. 什么情况下socket可读，可写

6. UDP协议和TCP协议的区别

TCP: bind

UDP: 需要bind吗，可以connect吗（可以然后可以调用send，否则要调用sendto指定目的地址和目的端口）

7. SO_REUSEADDR和SO_REUSEPORT区别

 A TCP/UDP connection is identified by a tuple of five values:

{<protocol>, <src addr>, <src port>, <dest addr>, <dest port>}

Any unique combination of these values identifies a connection. As a result, no two connections can have the same five values, otherwise the system would not be able to distinguish these connections any longer.


SO_REUSEADDR和SO_REUSEPORT主要是影响socket绑定ip和port的成功与否。先简单说几点绑定规则
规则1：socket可以指定绑定到一个特定的ip和port，例如绑定到192.168.0.11:9000上；
规则2：同时也支持通配绑定方式，即绑定到本地"any address"（例如一个socket绑定为 0.0.0.0:21，那么它同时绑定了所有的本地地址）；
规则3：默认情况下，任意两个socket都无法绑定到相同的源IP地址和源端口。

SO_REUSEADDR的作用主要包括两点
a. 改变了通配绑定时处理源地址冲突的处理方式
b. 改变了系统对处于TIME_WAIT状态的socket的看待方式

SO_REUSEPORT
a. 打破了上面的规则3，允许将多个socket绑定到相同的地址和端口，前提每个socket绑定前都需设置SO_REUSEPORT
b. Linux 内核3.9加入了SO_REUSEPORT

https://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t
http://www.jianshu.com/p/141aa1c41f15

8. TCP协议的粘包问题如何解决

9. UDP相关
UDP数据包的接收
client发送两次UDP数据，第一次 500字节，第二次300字节，server端阻塞模式下接包，第一次recvfrom( 1000 )，收到是 1000，还是500，还是300，还是其他？
由于UDP通信的有界性，接收到只能是500或300，又由于UDP的无序性和非可靠性，接收到可能是300，也可能是500，也可能一直阻塞在recvfrom调用上，直到超时返回(也就是什么也收不到)。
在假定数据包是不丢失并且是按照发送顺序按序到达的情况下，server端阻塞模式下接包，先后三次调用：recvfrom( 200)，recvfrom( 1000)，recvfrom( 1000)，接收情况如何呢？
由于UDP通信的有界性，第一次recvfrom( 200)将接收第一个500字节的数据包，但是因为用户空间buf只有200字节，于是只会返回前面200字节，剩下300字节将丢弃。第二次recvfrom( 1000)将返回300字节，第三次recvfrom( 1000)将会阻塞。

UDP包分片问题
如果MTU是1500，Client发送一个8000字节大小的UDP包，那么Server端阻塞模式下接包，在不丢包的情况下，recvfrom(9000)是收到1500，还是8000。如果某个IP分片丢失了，recvfrom(9000)，又返回什么呢？
根据UDP通信的有界性，在buf足够大的情况下，接收到的一定是一个完整的数据包，UDP数据在下层的分片和组片问题由IP层来处理，提交到UDP传输层一定是一个完整的UDP包，那么recvfrom(9000)将返回8000。如果某个IP分片丢失，udp里有个CRC检验，如果包不完整就会丢弃，也不会通知是否接收成功，所以UDP是不可靠的传输协议，那么recvfrom(9000)将阻塞。

UDP丢包问题
在不考虑UDP下层IP层的分片丢失，CRC检验包不完整的情况下，造成UDP丢包的因素有哪些呢？
[1] UDP socket缓冲区满造成的UDP丢包
通过 cat /proc/sys/net/core/rmem_default 和cat /proc/sys/net/core/rmem_max可以查看socket缓冲区的缺省值和最大值。如果socket缓冲区满了，应用程序没来得及处理在缓冲区中的UDP包，那么后续来的UDP包会被内核丢弃，造成丢包。在socket缓冲区满造成丢包的情况下，可以通过增大缓冲区的方法来缓解UDP丢包问题。但是，如果服务已经过载了，简单的增大缓冲区并不能解决问题，反而会造成滚雪球效应，造成请求全部超时，服务不可用。
[2] UDP socket缓冲区过小造成的UDP丢包
如果Client发送的UDP报文很大，而socket缓冲区过小无法容下该UDP报文，那么该报文就会丢失。
[3] ARP缓存过期导致UDP丢包
ARP的缓存时间约10分钟，APR缓存列表没有对方的MAC地址或缓存过期的时候，会发送ARP请求获取MAC地址，在没有获取到MAC地址之前，用户发送出去的UDP数据包会被内核缓存到arp_queue这个队列中，默认最多缓存3个包，多余的UDP包会被丢弃。被丢弃的UDP包可以从/proc/net/stat/arp_cache的最后一列的unresolved_discards看到。当然我们可以通过echo 30 > /proc/sys/net/ipv4/neigh/eth1/unres_qlen来增大可以缓存的UDP包。
UDP的丢包信息可以从cat /proc/net/udp 的最后一列drops中得到，而倒数第四列inode是丢失UDP数据包的socket的全局唯一的虚拟i节点号，可以通过这个inode号结合lsof(lsof -P -n | grep 25445445)来查到具体的进程。

10. What does “connection reset by peer” mean?

> This means that a TCP RST was received and the connection is now closed. This occurs when a packet is sent from your end of the connection but the other end does not recognize the connection; it will send back a packet with the RST bit set in order to forcibly close the connection.

This can happen if the other side crashes and then comes back up or if it calls close() on the socket while there is data from you in transit, and is an indication to you that some of the data that you previously sent may not have been received.

It is up to you whether that is an error; if the information you were sending was only for the benefit of the remote client then it may not matter that any final data may have been lost. However you should close the socket and free up any other resources associated with the connection.

https://stackoverflow.com/questions/1434451/what-does-connection-reset-by-peer-mean
https://everything2.com/title/Connection+reset+by+peer
http://blog.csdn.net/factor2000/article/details/3929816


-----------------
## Linux系统编程部分

1. 如何创建一个守护进程

2. 一个动态库可以多次dlopen吗

3. 如何查看ipc的一些资源
ipcs -a

4. 如何查看进程执行的系统调用
strace -pxxx -s1024

5. 如何查看进程打开的fd
ls -l /proc/pid/fd

6. 一个进程core了如何定位
gdb program core.xxx
bt
f 0

7. 多进程和多线程的区别

8. Linux虚拟地址空间如何分布？32位和64位有何不同？32/64位操作系统一个进程的虚拟地址空间分别是多少
4G/128T

9. malloc是如何分配内存的？malloc分配多大的内存，就占用多大的物理内存空间吗？free 的内存真的释放了吗（还给 OS ）? 既然堆内内存不能直接释放，为什么不全部使用 mmap 来分配？
malloc,calloc,realloc函数之间的区别？
```cpp
void * malloc(int n);
void * calloc(int n, int size);
void * realloc(void * p, int n);
```

10. 如何查看进程虚拟地址空间的使用情况？

11. 如何查看进程的缺页中断信息？
可通过以下命令查看缺页中断信息
ps -o majflt,minflt -C <program_name>
ps -o majflt,minflt -p <pid>
其中， majflt 代表 major fault ，指大错误， minflt 代表 minor fault ，指小错误。这两个数值表示一个进程自启动以来所发生的缺页中断的次数。其中 majflt 与 minflt 的不同是， majflt 表示需要读写磁盘，可能是内存对应页面在磁盘中需要 load 到物理内存中，也可能是此时物理内存不足，需要淘汰部分物理页面至磁盘中。
如果进程的内核态 CPU 使用过多，其中一个原因就可能是单位时间的缺页中断次数多个，可通过以上命令来查看。
如果 MAJFLT 过大，很可能是内存不足。
如果 MINFLT 过大，很可能是频繁分配 / 释放大块内存 (128k) ， malloc 使用 mmap 来分配。对于这种情况，可通过 mallopt(M_MMAP_THRESHOLD, <SIZE>) 增大临界值，或程序实现内存池。

12. 如何查看堆内内存的碎片情况？

13. 除了glibc的malloc/free ，还有其他第三方实现吗？

14. The difference among VIRT, RES, and SHR in `top` output

`VIRT` stands for the virtual size of a process, which is the sum of memory it is actually using, memory it has mapped into itself (for instance the video card’s RAM for the X server), files on disk that have been mapped into it (most notably shared libraries), and memory shared with other processes. VIRT represents how much memory the program is able to access at the present moment.

`RES` stands for the resident size, which is an accurate representation of how much actual physical memory a process is consuming. (This also corresponds directly to the %MEM column.) This will virtually always be less than the VIRT size, since most programs depend on the C library.

`SHR` indicates how much of the VIRT size is actually sharable (memory or libraries). In the case of libraries, it does not necessarily mean that the entire library is resident. For example, if a program only uses a few functions in a library, the whole library is mapped and will be counted in VIRT and SHR, but only the parts of the library file containing the functions being used will actually be loaded in and be counted under RES.

More:

http://mugurel.sumanariu.ro/linux/the-difference-among-virt-res-and-shr-in-top-output/

https://serverfault.com/questions/138427/top-what-does-virtual-memory-size-mean-linux-ubuntu

https://events.linuxfoundation.org/sites/events/files/slides/elc_2016_mem.pdf

15. IPC

sysv共享内存
shmget, shmat, shmdt
注意：每次shmat使用共享内存之后要shmdt，否则会导致打开句柄泄露（Too many open files in system(23)），通过ipcs -m可以看到nattch列有多少次attach。
``` cpp
// 如果已存在就使用之前的共享内存，如果不存在则创建共享内存并初始化
int shmid = shmget(MY_SHM_ID, sizeof(int), IPC_CREAT | 0666);
if (shmid == -1) {
	printf("shmget failed, %s(%d,id=%x)", strerror(errno), errno, MY_SHM_ID);
	return FAIL;
}
unsigned int * p_sn = (unsigned int *)shmat(shmid, NULL, 0);
if (p_sn == (unsigned int *) - 1) {
	printf("shmat failed, %s(%d,id=%x)", strerror(errno), errno, MY_SHM_ID);
	return FAIL;
}
unsigned old_sn = __sync_fetch_and_add(p_sn, 1);
if (shmdt(p_sn) == -1) {
	printf("shmdt failed, %s(%d,id=%x)", strerror(errno), errno, MY_SHM_ID);
	return FAIL;
}
```
http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html

16. wait命令

wait命令一个很重要用途就是在Bash shell的并行编程中，可以在Bash shell脚本中启动多个后台进程（使用&），然后调用wait命令，等待所有后台进程都运行完毕，Bash shell脚本再继续向下执行。
```bash
command1 &
command2 &
wait
```

17. Linux上的内存如计算？

`top`:
```
Mem:  131997524k total, 130328500k used,  1669024k free,   793232k buffers
Swap:  2105272k total,   428816k used,  1676456k free, 122989268k cached
```

`free -m`
```
             total       used       free     shared    buffers     cached
Mem:        128903     128567        336          0        776     121401
-/+ buffers/cache:       6389     122514
Swap:         2055        418       1637
```

可用内存：
122514 （-/+ buffers/cache free） = 336 （free）+ 776 （buffers）+ 121401 （cached）
总内存：
128902 （Mem: total） = 6389 （-/+ buffers/cache used）+ 122514 （-/+ buffers/cache free）

在很多Linux服务器上运行free 命令，会发现剩余内存（Mem:行的free列）很少，但实际服务器上的进程并没有占用很大的内存。这是因为Linux特殊的内存管理机制。Linux内核会把空闲的内存用作buffer/cached，用于提高文件读取性能。当应用程序需要用到内存时，buffer/cached内存是可以马上回收的。所以，对应用程序来说，buffer/cached是可用的，可用内存应该是free+buffers+cached。因为这个原因，free命令也才有第三行的-/+ buffers/cache。

Linux内存占用分析，是一个比较复杂的主题。更多资料可以参考下面的系列文章：

https://techtalk.intersec.com/2013/07/memory-part-1-memory-types/

https://techtalk.intersec.com/2013/07/memory-part-2-understanding-process-memory/

https://techtalk.intersec.com/2013/08/memory-part-3-managing-memory/

https://techtalk.intersec.com/2013/10/memory-part-4-intersecs-custom-allocators/

https://techtalk.intersec.com/2013/12/memory-part-5-debugging-tools/


18. Makefile

一个工程中的源文件不计其数，其按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作。

比如有一个test.c文件，gcc针对上面几个过程，对应的命令如下：
```
预处理：gcc -E test.c -o test.i

编译：gcc -S test.c -o test.s

汇编：gcc -c test.s -o test.o

连接：gcc test.o -o test
```

并行编译。工程比较大时，并行编译非常会充分利用多CPU的优势，缩短编译的时间。
```
make -f makefile -j 8
```

两个函数patsubst，wildcard

makefile函数定义格式为：$(<function><arguments>)。<function>就是函数名，make支持的函数不多。<arguments>是函数的参数，参数间以逗号“,”分隔，而函数名和参数之间以“空格”分隔
```
$(wildcard *.cpp)：表示展开工作目录下所有的.cpp文件；
$(patsubst %.cpp,%.o,$(wildcard *.cpp))：表示对.cpp文件列表字符串进行替换，替换为.o文件后缀的列表字符串；
```

Makefile.comm
``` 
CXX = g++
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS_I = $(patsubst %.cpp,%.i,$(wildcard *.cpp))
DEPENDS = $(patsubst %.o,%.d,$(OBJS))

MYFLAGS = -MMD -Wno-unused -pipe -g -O2 -D_GNU_SOURCE
PLATFORM := $(strip $(shell echo `uname -m`))
ifeq ($(PLATFORM,x86_64))
    PLAT_FLAGS := 64
else
    PLAT_FLAGS := 32
endif
MYFLAGS += -m${PLAT_FLAGS}
```

常用编译选项

-MMD：生成文件关联信息，并将结果保存在.d文件中；我们可以很方便的分析，引用的.h文件目录是否正确；

-Wno_unused：对于定义了没有使用的变量是否需要告警出来；

-pipe：使用管道代替编译中的临时文件；这是gcc的优化参数，默认情况下，gcc编译过程中的中间临时文件会放到/tmp目录下，并在编译完成后删除掉；读写文件会影响到编译的效率，管道取代临时文件，提升了编译效率；

-g：产生调试信息；调试信息分级别，默认是g2，也可以提升为g3，这样包含定义的宏；

-02：编译器的优化选项的4个级别，-O0表示没有优化,-O1为缺省值，-O3优化级别最高；

-D_GNU_SOURCE：表示程序在GNU标准下进行编译，如果用到了GNU标准的信号量，锁等API函数，需要加上该选项；

-m32：生成32位操作系统的汇编代码；

$(strip $(shell  echo `uname -m`))：打印操作系统版本出来；

Makefile
```
include ./Makefile.comm

TARGET = your_bin
PROJ_DIR = $(shell pwd)
SRC_DIR = ./
OBJ_DIR = ./

.PHONY: all clean init

all: $(OBJS_I) $(TARGET)
    @echo -e "make proj dir $(PROJ_DIR) success"

$(TARGET): $(OBJS)
    $(CXX) $(MYFLAGS) $^ -o $@ $(LIB)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
    $(CXX) $(MYFLAGS) -c -o $@ $(INC) $<

$(OBJ_DIR)%.i: $(SRC_DIR)%.cpp
    $(CXX) $(MYFLAGS) -o $@ $(INC) -E $<

clean:
    rm -f $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)
```
PHONY：定义了显示执行makefile的命令名称，告诉编译系统，all，clean，init假定这些目标需要更新。有两个作用：a）如果clean为一个文件时，make clean将失效；b）如果all后面加上clean，但clean未声明为PHONY，编译系统会认为clean没有依赖，clean文件已经是最新的，不需要执行；

可以看见makefile的第一个目标为all，因此只执行make不加目标名的时候，将执行该伪目标；
用到了makefile常用的几个通配符，$^ $@, $<
$@：目标文件
$^：所有的依赖文件
$<：第一个依赖文件
include表示包含一个外部的makefile文件进来，-include和include功能一样，-include忽略文件不存在的报错；

通常我们在Makefile中可使用“-include”来代替“include”，来忽略由于包含文件不存在或者无法创建时的错误提示（“-”的意思是告诉make，忽略此操作的错误。make继续执行）。像下边那样：
-include FILENAMES...
使用这种方式时，当所要包含的文件不存在时不会有错误提示、make也不会退出；除此之外，和第一种方式效果相同。以下是这两种方式的比较：
使用“include FILENAMES...”，make程序处理时，如果“FILENAMES”列表中的任何一个文件不能正常读取而且不存在一个创建此文件的规则时make程序将会提示错误并退出。
使用“-include FILENAMES...”的情况是，当所包含的文件不存在或者不存在一个规则去创建它，make程序会继续执行，只有真正由于不能正确完成终极目标的重建时（某些必需的目标无法在当前已读取的makefile文件内容中找到正确的重建规则），才会提示致命错误并退出。
为了和其它的make程序进行兼容。也可以使用“sinclude”来代替“-include”（GNU所支持的方式）。

-----------------
## 数据库部分

1. Locking Reads
select ... for update

Locking of rows for update using SELECT FOR UPDATE only applies when autocommit is disabled (either by beginning transaction with START TRANSACTION or by setting autocommit to 0. If autocommit is enabled, the rows matching the specification are not locked.

https://stackoverflow.com/questions/10935850/when-to-use-select-for-update
https://dev.mysql.com/doc/refman/5.7/en/innodb-locking-reads.html

2. mysql_use_result和mysql_store_result的区别

3. B+Tree和LSM Tree区别
nosql基本没有用B+树的，很多采用了LSM Tree，比如hbase/cassandra，rocksdb/leveldb
B+树跟LSM Tree的时间复杂度对比（N是tree的node数）
随机点写入，LSM Tree O(1)，B+树O(logN)
随机点读出，LSM Tree O(N)，B+树O(logN)
对B+树或者LSM Tree做一些变形增加一些数据结构，也可以优化一些时间复杂度，当然这些优化可能带来额外的开销，更多的时候是在读/写、随机/顺序之间的tradeoff，有些专门的论文研究这方面的数据结构和算法。有兴趣的可以看看tokudb。
传统OLTP数据库在机械磁盘这种慢设备（随机IO性能差很多）上设计的，OLTP场景本来也是读多写少，数据量不大，因此，选择B+数据的数据结构主要是这些历史原因吧。传统数据库的存储引擎非常复杂（存储引擎和查询优化是单机RDBMS的两个核心难点）。此外，即使在SSD上，随机IO跟顺序IO的性能差异也是有的。因此，SSD出现后，传统RDBMS，也没有动力去重构存储引擎。当然，mysql是出现了myrocks这样的项目作为LSM tree的存储引擎。
随着数据量的爆炸和SSD的出现，越来越多的新兴数据库选择LSM Tree，这样数据写入性能有较大提升，用LSM tree做列存储，数据压缩效果比行存的B+树也要好很多。同时由于SSD的存在，随机读性能也比B+树也不会差太多。

一般来说LSMtree更适合分析，比如经常做批量数据倒入，范围顺序扫描等场景；B+树更适合OLTP的点查询。LSMTree是列存储友好的（value非定长，容易压缩），B+树是行存储友好的（字段定长）。行存储更多用在事务库，列存储更多是分析库吧。有人提出列存储和行存的混合模式做HTAP的数据库，论文记不清了。对于未来趋势，最近开源出来的OLTP的newsql（cockroach tidb），都是用LSMTree。

https://www.zhihu.com/question/65628840

-----------------
## 算法和数据结构

1. 10亿数据中取最大的100个数据

http://www.jianshu.com/p/4427db9337d7

2. HashMap的实现原理

数组 + 链表（桶）。简单地说，HashMap 在底层将 key-value 当成一个整体进行处理，这个整体就是一个 Entry 对象。HashMap 底层采用一个 Entry[] 数组来保存所有的 key-value 对，当需要存储一个 Entry 对象时，会根据 hash 算法来决定其在数组中的存储位置，在根据 equals 方法决定其在该数组位置上的链表中的存储位置；当需要取出一个Entry 时，也会根据 hash 算法找到其在数组中的存储位置，再根据 equals 方法从该位置上的链表中取出该Entry。

``` java
public HashMap(int initialCapacity, float loadFactor) {
        if (initialCapacity < 0)
            throw new IllegalArgumentException("Illegal initial capacity: " +
                                               initialCapacity);
        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;
        if (loadFactor <= 0 || Float.isNaN(loadFactor))
            throw new IllegalArgumentException("Illegal load factor: " +
                                               loadFactor);

        // Find a power of 2 >= initialCapacity
        int capacity = 1;
        while (capacity < initialCapacity)
            capacity <<= 1;

        this.loadFactor = loadFactor;
        threshold = (int)Math.min(capacity * loadFactor, MAXIMUM_CAPACITY + 1);
        table = new Entry[capacity]; // 数组
        useAltHashing = sun.misc.VM.isBooted() &&
                (capacity >= Holder.ALTERNATIVE_HASHING_THRESHOLD);
        init();
}

static class Entry<K,V> implements Map.Entry<K,V> {
    final K key;      // 一个键值对
    V value;
    Entry<K,V> next;  // next 的 Entry 指针（桶）
    final int hash;
    ……
}

// 核心算法
static int hash(int h) { 
        h ^= (h >>> 20) ^ (h >>> 12); 
        return h ^ (h >>> 7) ^ (h >>> 4); 
} 

static int indexFor(int h, int length) { 
        return h & (length-1); 
 } 
```
[Why initialCapacity of Hashtable is 11 while the DEFAULT_INITIAL_CAPACITY in HashMap is 16 and requires a power of 2](https://stackoverflow.com/questions/9413966/why-initialcapacity-of-hashtable-is-11-while-the-default-initial-capacity-in-has)

[HashMap requires a better hashCode() - JDK 1.4 Part II](http://www.javaspecialists.eu/archive/Issue054.html)

hashtable使用质数是考虑到分布更均匀，但模运算比较慢。hashmap使用2的指数可以利用掩码运算速度更快，但是设计了新的rehash方法，总体性能比之前好。

如何保证元素均匀，特殊的取模方法，但是模运算的计算代价高，h%length改为h&(length-1)，要求length是2^n。当数组长度为 2 的 n 次幂的时候，不同的 key 算得得 index 相同的几率较小，那么数据在数组上分布就比较均匀，也就是说碰撞的几率小，相对的，查询的时候就不用遍历某个位置上的链表，这样查询效率也就较高了。

HashMap 的 resize（rehash）？当 HashMap 中的元素越来越多的时候，hash 冲突的几率也就越来越高，因为数组的长度是固定的。所以为了提高查询的效率，就要对 HashMap 的数组进行扩容。而在 HashMap 数组扩容之后，最消耗性能的点就出现了：原数组中的数据必须重新计算其在新数组中的位置，并放进去，这就是 resize。所以如果我们已经预知 HashMap 中元素的个数，那么预设元素的个数能够有效的提高 HashMap 的性能。

线程安全？在Java里的解决方法是：使用java.util.HashTable，效率最低；或者使用java.util.concurrent.ConcurrentHashMap，相对安全，效率较高。

http://wiki.jikexueyuan.com/project/java-collection/hashmap.html

http://wiki.jikexueyuan.com/project/java-collection/hashtable.html

http://zhaox.github.io/2016/07/05/hashmap-vs-hashtable

3. KMP算法

KMP算法的时间复杂度为0（m+n），这个算法最为巧妙的地方在于：每次匹配时，利用了之前匹配的结果，从而避免需要将指针回退。

子串的next数组实际是这个算法的cache数据，牺牲了空间，换取了时间上的高效，在我们实际项目中，有很多类似的应用场景；

另外一个经典的字符串匹配算法AC自动机匹配算法，用于对一段Query进行字典词匹配。在实际应用场景中，如基于字典表的分词，脏词库识别等，都有广泛应用，后面也将展开该算法的实现。


4. 无锁队列的实现

数组
链表

5. 怎样写出一个更快的 memset/memcpy

https://www.zhihu.com/question/35172305

-----------------
## 内核相关

1. Linux的RCU机制


2. RBP 和 RSP
X86-64 64位的寄存器一般都是叫RXX，其低32位向下兼容一般称为EXX

不过x86-64时代，现代编译器下已经不是这样的布局了，传递参数不再压栈，而是通过固定的几个寄存器。

现代编译器做了这些改变，除了通过寄存器传递参数比之前参数压栈要快之外。对于我们debug好处，就是当出现堆栈溢出，向上冲破到FunA的栈帧的时候，不会覆盖到这些参数，那么core的时候我们依然可以看到FunA传递给FunB的参数，这样我们就能够定位到底是不是由于FunA传递进来的参数引起的。

```
# 查看RBP指向的区域
p $rbp
x/64ag $rbp
```

局部变量都在栈底rbp的下方，即$rbp-Offset的区域。如果知道了对应的Offset值，不就能够p去查看了

CPU常用寄存器

在X86-64中，所有寄存器都是64位，相对32位的x86来说，标识符发生了变化，比如：从原来的%ebp变成了%rbp。为了向后兼容性，%ebp依然可以使用，不过指向了%rbp的低32位。

X86-64寄存器的变化，不仅体现在位数上，更加体现在寄存器数量上。新增加寄存器%r8到%r15。加上x86的原有8个，一共16个寄存器。

| 寄存器 | 作用
| -- | --
| RIP | 指向哪，程序就运行到哪里。调用FunB，就把RIP指向FunB的入口地址。
| RAX | 函数的返回值一般通过RAX寄存器传递
| RBP | 一般用于指向当前函数调用栈的**栈底**，以方便对栈的操作,如获取函数参数、局部变量等。低32位称为EBP
| RSP | 一直指向当前函数调用栈的**栈顶**，RSP在函数返回时要和调用前的指向要一致，即使所谓的栈平衡。低32位叫ESP

传递参数用到的寄存器，看反汇编时会遇到

| 寄存器 | 作用
| -- | --
| RDI | 第1个参数
| RSI | 第2个参数
| RDX | 第3个参数
| RCX | 第4个参数
| R8 | 第5个参数
| R9 | 第6个参数

使用gdb 指令info reg 可以查看CPU常用的几个寄存器。对其他寄存器感兴趣的同学可以查看《参考文献》当中《Intel® 64 and IA-32 Architectures Software Developer Manuals》和《X86-64寄存器和栈帧》

反汇编

gdb 指令 disassemble /m 函数名(或者地址)， /m是映射到代码

gdb 默认使用的AT&T的语法，另外一种常见的是Intel 汇编语法（个人更喜欢这种风格）

可以使用set disassembly-flavor intel or att来切换风格


参考文献

函数调用 压栈的工作原理
http://blog.csdn.net/u011555996/article/details/70211315

X86-64寄存器和栈帧
http://blog.csdn.net/dayancn/article/details/51328959

对于ESP、EBP寄存器的理解
http://blog.csdn.net/yeruby/article/details/39780943

esp和ebp详解
http://www.cnblogs.com/dormant/p/5079894.html

GDB相关反汇编指令
http://blog.csdn.net/linuxheik/article/details/17529919

LINUX下GDB反汇编和调试
http://www.cnblogs.com/brucemengbm/p/7223724.html

AT&T汇编语法
http://ted.is-programmer.com/posts/5251.html

AT&T汇编简介
http://ilovers.sinaapp.com/article/att%E6%B1%87%E7%BC%96gasgnu-assembly

AT&T 与 Intel 汇编语言的比较
http://blog.csdn.net/21aspnet/article/details/7176915

GCC 中的编译器堆栈保护技术
https://www.ibm.com/developerworks/cn/linux/l-cn-gccstack/index.html

Linux下缓冲区溢出攻击的原理及对策
https://www.ibm.com/developerworks/cn/linux/l-overflow/

缓冲区溢出攻击
http://netsecurity.51cto.com/art/201407/446830.htm

GCC “Options That Control Optimization”
https://gcc.gnu.org/onlinedocs/gcc-4.4.2/gcc/Optimize-Options.html

Stack overflow “gcc -fomit-frame-pointer”
https://stackoverflow.com/questions/14666665/trying-to-understand-gcc-option-fomit-frame-pointer

Intel® 64 and IA-32 Architectures Software Developer Manuals
https://software.intel.com/en-us/articles/intel-sdm

一种处理栈越界的方法
https://zhuanlan.zhihu.com/p/20642841


-----------------
## 硬件相关

1. 如何理解SSD的写放大

https://www.zhihu.com/question/31024021


-----------------
## 数学

1. 已知某酒鬼有90%的日子都会出去喝酒，喝酒只去固定三家酒吧。今天警察找了其中两家酒吧都没有找到酒鬼。问：酒鬼在第三家酒吧的几率？
https://www.guokr.com/post/61605/


-----------------
## 性能调优

1. the zero-cost exception model
It offers zero runtime overhead for an error-free execution, which surpasses even that of C return-value error handling. As a trade-off, the propagation of an exception has a large overhead.

```
Exception Frames

Exceptions are handled in frames. The simplest of these frames is a ‘try/catch’ block in the source code. The ‘try’ opens an exception frame and the ‘catch’ defines the handler for that frame. Exceptions within that frame flow to the handler: either they match and the exception is handled, or they do not match and the exception is propagated further.

In addition to ‘catch’ handlers, are cleanup handlers. This is code that must be called during exception propagation but doesn’t stop the exception. This includes any ‘finally’ blocks and C++ destructors. Explicit ‘finally’ blocks are matched with a ‘try’, so it is easy to see where their frame is defined. Destructors create implicit frames, starting at the point of instantiation (variable declaration), ending with the enclosing scope. Note that every object with a destructor creates its own exception frame.

Languages with ‘defer’ statements, like Leaf, also introduce exception frames. These are very similar to destructors: the ‘defer’ statement opens a frame which ends with the enclosing scope. Any language feature which requires code to execute during an exception will require an exception frame.

**It’s important to see all of these frames; compilers used to generate code at each ‘try’ statement to handle exceptions. One technique was ‘setjmp/longjmp’: each ‘try’ adds a jump address to an exception handler stack and the ‘finally’ clauses remove it. Even if no exception was thrown the work of adding/removing from the handler stack would still be done. One of the key goals of zero-cost exceptions was to get rid of this needless setup/teardown work.**
```

https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/
http://ithare.com/infographics-operation-costs-in-cpu-clock-cycles/

2. gettimeofday优化
https://access.redhat.com/solutions/18627
cat /sys/devices/system/clocksource/clocksource0/available_clocksource
echo "acpi_pm" > /sys/devices/system/clocksource/clocksource0/current_clocksource
