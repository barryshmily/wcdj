
---
[TOC]

## Codes

[使用异步 I/O 大大提高应用程序的性能](https://www.ibm.com/developerworks/cn/linux/l-async/)


## Tools

### perf


[perf CPU Sampling](http://www.brendangregg.com/blog/2014-06-22/perf-cpu-sample.html)

[Flame Graphs](http://www.brendangregg.com/perf.html#FlameGraphs)

[FlameGraph](https://github.com/brendangregg/FlameGraph)

[Linux kernel profiling with perf](https://perf.wiki.kernel.org/index.php/Tutorial)

编译选项： -fno-omit-frame-pointer

```
# sudo perf record -F 99 -a -g -- sleep 20
# sudo perf report -n --stdio
```

demo
https://github.com/brendangregg/FlameGraph/issues/47#issuecomment-76298637

``` c
#include <unistd.h>

int compute(int start) {
|   for (int i = 0; i < 10000000; ++i) {
|   |   start += i;
|   }
|   return start;
}

int main(int argc, char **argv) {
|   int ret = 0;
|   for (int i = 0; i != 5; ++i) {
|   |   sleep(argc == 1 ? 0 : 1);
|   |   ret += compute(argc);
|   }
|   return ret;
}
```

方法1：使用FlameGraph查看不到nanosleep的调用

# sudo perf record -F 500 -g --call-graph dwarf -- ./perf  && sudo perf script | /root/FlameGraph/stackcollapse-perf.pl | /root/FlameGraph/flamegraph.pl > perf.svg

方法2：

https://github.com/brendangregg/FlameGraph/issues/47#issuecomment-76298637

方法3：

root@ubuntu-s-1vcpu-3gb-nyc3-01:~/test# strace -c ./perf -Ttt
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 23.81    0.000025           5         5           nanosleep
 17.14    0.000018           3         7           mmap
 15.24    0.000016           4         4           mprotect
  8.57    0.000009           5         2           open
  7.62    0.000008           8         1           munmap
  7.62    0.000008           3         3         3 access
  3.81    0.000004           2         2           close
  3.81    0.000004           2         2           fstat
  3.81    0.000004           0        10           rt_sigprocmask
  1.90    0.000002           2         1           read
  1.90    0.000002           0         5           rt_sigaction
  1.90    0.000002           2         1           execve
  1.90    0.000002           2         1           arch_prctl
  0.95    0.000001           1         1           brk
------ ----------- ----------- --------- --------- ----------------
100.00    0.000105                    45         3 total



### mtr


### iostat

iostat -dx

iostat -dx 1

iostat -dx <disk name> 1

[How to get total read and total write IOPS in Linux?](https://unix.stackexchange.com/questions/225095/how-to-get-total-read-and-total-write-iops-in-linux)

### bmon

bmon - bandwidth monitor and rate estimator

### dd

[使用 dd 命令进行硬盘 I/O 性能检测](https://linux.cn/article-6104-1.html)

```
[root@10 ~]# dd if=/dev/zero of=/tmp/gerry bs=1G count=1 oflag=direct
1+0 records in
1+0 records out
1073741824 bytes (1.1 GB) copied, 6.22015 s, 173 MB/s
```

注意：虚拟机dd测试本地盘，是虚拟化之后的磁盘，这个会受资源隔离的影响

### fio

测试案例：

CBS test

https://github.com/bingjin/CloudTesting/blob/master/test_cbs/README.md

**fio - flexible I/O tester**

https://linux.die.net/man/1/fio

> fio is a tool that will spawn a number of threads or processes doing a particular type of I/O action as specified by the user. The typical use of fio is to write a job file matching the I/O load one wants to simulate.

Refer:

[使用fio测试磁盘I/O性能](ttps://wsgzao.github.io/post/fio/)

[Fio压测工具和io队列深度理解和误区](http://blog.yufeng.info/archives/2104)

https://github.com/bingjin/CloudTesting/blob/master/test_cbs/test_procedure.md

[队列深度对磁盘IOPS的影响](http://www.ksyun.com/blog/dui-lie-shen-du-dui-ci-pan-iopsde-ying-xiang/)

[FIO输出字段说明](http://www.ksyun.com/blog/fioshu-chu-zi-duan-shuo-ming/)

```
#yum安装
yum install libaio-devel fio
#手动安装
yum install libaio-devel
wget http://brick.kernel.dk/snaps/fio-3.5.tar.gz
tar -zxvf fio-3.5.tar.gz
cd fio-3.5
make && make install
```

fio参数：

```
filename=/dev/sdb1   #测试文件名称，通常选择需要测试的盘的data目录
direct=1             #测试过程绕过机器自带的buffer。使测试结果更真实
rw=randwrite         #测试随机写的I/O
rw=randrw            #测试随机写和读的I/O
bs=16k               #单次io的块文件大小为16k
bsrange=512-2048     #同上，提定数据块的大小范围
size=5G              #本次的测试文件大小为5g，以每次4k的io进行测试
numjobs=30           #本次的测试线程为30个
runtime=1000         #测试时间1000秒，如果不写则一直将5g文件分4k每次写完为止
ioengine=psync       #io引擎使用psync方式
rwmixwrite=30        #在混合读写的模式下，写占30%
group_reporting      #关于显示结果的，汇总每个进程的信息
lockmem=1G           #只使用1g内存进行测试
zero_buffers         #用0初始化系统buffer
nrfiles=8            #每个进程生成文件的数量
```

fio测试：

测试前清理缓存。
echo 3 > /proc/sys/vm/drop_caches


1. 命令行

腾讯云标准测试

```
# IOPS-randread
fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=randread -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-randread

# Throughput-randwrite
# 128k
fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=randwrite -ioengine=libaio -bs=128k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-randwrite

# 4k
fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=randwrite -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-randwrite

# Latency-read
fio -filename=/dev/rbd0 -direct=1 -iodepth=1 -rw=read -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-read
```

顺序写

```
# IOPS-write
fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=write -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-write
```

云盘：

[root@10 tools]# fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=randread -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-randread
fio-randread: (g=0): rw=randread, bs=(R) 4096B-4096B, (W) 4096B-4096B, (T) 4096B-4096B, ioengine=libaio, iodepth=32
fio-3.5
Starting 1 process
Jobs: 1 (f=1): [r(1)][100.0%][r=63.1MiB/s,w=0KiB/s][r=16.2k,w=0 IOPS][eta 00m:00s]
fio-randread: (groupid=0, jobs=1): err= 0: pid=9368: Fri Mar  2 14:27:21 2018
   read: IOPS=12.0k, BW=50.7MiB/s (53.1MB/s)(14.8GiB/300002msec)
    slat (nsec): min=0, max=4565.1k, avg=6100.77, stdev=7697.51
    clat (usec): min=1488, max=1017.3k, avg=2457.88, stdev=15873.34
     lat (usec): min=1494, max=1017.3k, avg=2464.40, stdev=15873.35
    clat percentiles (usec):
     |  1.00th=[  1582],  5.00th=[  1647], 10.00th=[  1713], 20.00th=[  1844],
     | 30.00th=[  1876], 40.00th=[  1909], 50.00th=[  1926], 60.00th=[  1958],
     | 70.00th=[  2024], 80.00th=[  2114], 90.00th=[  2212], 95.00th=[  2311],
     | 99.00th=[  2802], 99.50th=[  3294], 99.90th=[139461], 99.95th=[467665],
     | 99.99th=[708838]
   bw (  KiB/s): min= 9608, max=66088, per=100.00%, avg=51896.40, stdev=13987.31, samples=600
   iops        : min= 2402, max=16522, avg=12974.09, stdev=3496.84, samples=600
  lat (msec)   : 2=67.35%, 4=32.30%, 10=0.17%, 20=0.03%, 50=0.03%
  lat (msec)   : 100=0.01%, 250=0.02%, 500=0.04%, 750=0.04%, 1000=0.01%
  cpu          : usr=3.87%, sys=12.16%, ctx=2121714, majf=0, minf=42
  IO depths    : 1=0.1%, 2=0.1%, 4=0.1%, 8=0.1%, 16=0.1%, 32=100.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.1%, 64=0.0%, >=64=0.0%
     issued rwts: total=3892137,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=32

Run status group 0 (all jobs):
   READ: bw=50.7MiB/s (53.1MB/s), 50.7MiB/s-50.7MiB/s (53.1MB/s-53.1MB/s), io=14.8GiB (15.9GB), run=300002-300002msec

Disk stats (read/write):
  rbd0: ios=3890492/0, merge=0/0, ticks=9445838/0, in_queue=9471765, util=100.00%


[root@10 tools]# fio -filename=/dev/rbd0 -direct=1 -iodepth=32 -rw=randwrite -ioengine=libaio -bs=128k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-randwrite
fio-randwrite: (g=0): rw=randwrite, bs=(R) 128KiB-128KiB, (W) 128KiB-128KiB, (T) 128KiB-128KiB, ioengine=libaio, iodepth=32
fio-3.5
Starting 1 process
Jobs: 1 (f=1): [w(1)][100.0%][r=0KiB/s,w=116MiB/s][r=0,w=927 IOPS][eta 00m:00s]
fio-randwrite: (groupid=0, jobs=1): err= 0: pid=10039: Fri Mar  2 14:38:35 2018
  write: IOPS=908, BW=114MiB/s (119MB/s)(33.3GiB/300295msec)
    slat (usec): min=4, max=26497, avg=19.46, stdev=73.38
    clat (usec): min=272, max=6396.3k, avg=35170.79, stdev=87838.45
     lat (msec): min=3, max=6396, avg=35.19, stdev=87.84
    clat percentiles (msec):
     |  1.00th=[    6],  5.00th=[    7], 10.00th=[    8], 20.00th=[    9],
     | 30.00th=[   10], 40.00th=[   11], 50.00th=[   13], 60.00th=[   16],
     | 70.00th=[   21], 80.00th=[   31], 90.00th=[   71], 95.00th=[  184],
     | 99.00th=[  305], 99.50th=[  422], 99.90th=[  684], 99.95th=[  986],
     | 99.99th=[ 3574]
   bw (  KiB/s): min= 5888, max=210176, per=100.00%, avg=117547.46, stdev=15862.43, samples=594
   iops        : min=   46, max= 1642, avg=918.31, stdev=123.92, samples=594
  lat (usec)   : 500=0.01%, 750=0.01%, 1000=0.01%
  lat (msec)   : 4=0.15%, 10=32.22%, 20=36.98%, 50=17.88%, 100=4.64%
  lat (msec)   : 250=6.30%, 500=1.51%, 750=0.22%, 1000=0.03%
  cpu          : usr=3.40%, sys=2.89%, ctx=248529, majf=0, minf=11
  IO depths    : 1=0.1%, 2=0.1%, 4=0.1%, 8=0.1%, 16=0.1%, 32=100.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.1%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,272759,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=32

Run status group 0 (all jobs):
  WRITE: bw=114MiB/s (119MB/s), 114MiB/s-114MiB/s (119MB/s-119MB/s), io=33.3GiB (35.8GB), run=300295-300295msec

Disk stats (read/write):
  rbd0: ios=0/272751, merge=0/217, ticks=0/9574461, in_queue=9579362, util=100.00%


[root@10 tools]# fio -filename=/dev/rbd0 -direct=1 -iodepth=1 -rw=read -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-read
fio-read: (g=0): rw=read, bs=(R) 4096B-4096B, (W) 4096B-4096B, (T) 4096B-4096B, ioengine=libaio, iodepth=1
fio-3.5
Starting 1 process
Jobs: 1 (f=1): [R(1)][100.0%][r=1956KiB/s,w=0KiB/s][r=489,w=0 IOPS][eta 00m:00s]
fio-read: (groupid=0, jobs=1): err= 0: pid=10646: Fri Mar  2 14:48:26 2018
   read: IOPS=530, BW=2121KiB/s (2172kB/s)(621MiB/300001msec)
    slat (usec): min=5, max=812, avg=10.91, stdev= 5.77
    clat (usec): min=1510, max=10264, avg=1870.90, stdev=226.82
     lat (usec): min=1518, max=10276, avg=1882.36, stdev=227.12
    clat percentiles (usec):
     |  1.00th=[ 1549],  5.00th=[ 1582], 10.00th=[ 1598], 20.00th=[ 1647],
     | 30.00th=[ 1811], 40.00th=[ 1844], 50.00th=[ 1860], 60.00th=[ 1876],
     | 70.00th=[ 1926], 80.00th=[ 2073], 90.00th=[ 2114], 95.00th=[ 2180],
     | 99.00th=[ 2245], 99.50th=[ 2376], 99.90th=[ 3163], 99.95th=[ 4555],
     | 99.99th=[ 8356]
   bw (  KiB/s): min= 1792, max= 2528, per=100.00%, avg=2121.35, stdev=194.02, samples=599
   iops        : min=  448, max=  632, avg=530.34, stdev=48.51, samples=599
  lat (msec)   : 2=75.82%, 4=24.13%, 10=0.06%, 20=0.01%
  cpu          : usr=0.33%, sys=0.91%, ctx=159071, majf=0, minf=12
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=159069,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
   READ: bw=2121KiB/s (2172kB/s), 2121KiB/s-2121KiB/s (2172kB/s-2172kB/s), io=621MiB (652MB), run=300001-300001msec

Disk stats (read/write):
  rbd0: ios=159022/0, merge=0/0, ticks=295176/0, in_queue=295112, util=98.42%


本地SAS盘

[root@10 ~]# fio -filename=/dev/vda -direct=1 -iodepth=32 -rw=write -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-write 
fio-write: (g=0): rw=write, bs=(R) 4096B-4096B, (W) 4096B-4096B, (T) 4096B-4096B, ioengine=libaio, iodepth=32
fio-3.5
Starting 1 process
Jobs: 1 (f=1): [W(1)][100.0%][r=0KiB/s,w=81.5MiB/s][r=0,w=20.9k IOPS][eta 00m:00s]
fio-write: (groupid=0, jobs=1): err= 0: pid=15759: Fri Mar  2 18:36:08 2018
  write: IOPS=20.5k, BW=80.2MiB/s (84.1MB/s)(23.5GiB/300002msec)
    slat (nsec): min=1970, max=3726.3k, avg=7958.47, stdev=11738.40
    clat (usec): min=11, max=2943.1k, avg=1546.49, stdev=7165.83
     lat (usec): min=59, max=2943.1k, avg=1555.11, stdev=7165.84
    clat percentiles (usec):
     |  1.00th=[   75],  5.00th=[   92], 10.00th=[  125], 20.00th=[ 1303],
     | 30.00th=[ 1467], 40.00th=[ 1549], 50.00th=[ 1614], 60.00th=[ 1663],
     | 70.00th=[ 1729], 80.00th=[ 1827], 90.00th=[ 1975], 95.00th=[ 2180],
     | 99.00th=[ 2900], 99.50th=[ 6521], 99.90th=[14746], 99.95th=[19530],
     | 99.99th=[45876]
   bw (  KiB/s): min= 4504, max=95448, per=100.00%, avg=82803.77, stdev=9873.08, samples=595
   iops        : min= 1126, max=23862, avg=20700.94, stdev=2468.27, samples=595
  lat (usec)   : 20=0.01%, 50=0.01%, 100=6.83%, 250=5.72%, 500=0.64%
  lat (usec)   : 750=0.73%, 1000=1.29%
  lat (msec)   : 2=76.00%, 4=8.17%, 10=0.33%, 20=0.24%, 50=0.04%
  lat (msec)   : 100=0.01%, 250=0.01%, 500=0.01%, 750=0.01%
  cpu          : usr=10.10%, sys=23.89%, ctx=4321217, majf=0, minf=36
  IO depths    : 1=0.1%, 2=0.1%, 4=0.1%, 8=0.1%, 16=0.1%, 32=100.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.1%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,6158633,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=32

Run status group 0 (all jobs):
  WRITE: bw=80.2MiB/s (84.1MB/s), 80.2MiB/s-80.2MiB/s (84.1MB/s-84.1MB/s), io=23.5GiB (25.2GB), run=300002-300002msec

Disk stats (read/write):
  vda: ios=3520/6162861, merge=0/1888, ticks=36821/9505825, in_queue=9541709, util=100.00%


[root@10 ~]# fio -filename=/dev/vda -direct=1 -iodepth=1 -rw=read -ioengine=libaio -bs=4k -size=100G --time_based -runtime=300 --refill_buffers --norandommap --randrepeat=0 -group_reporting -name=fio-read 
fio-read: (g=0): rw=read, bs=(R) 4096B-4096B, (W) 4096B-4096B, (T) 4096B-4096B, ioengine=libaio, iodepth=1
fio-3.5
Starting 1 process
Jobs: 1 (f=1): [R(1)][100.0%][r=30.5MiB/s,w=0KiB/s][r=7802,w=0 IOPS][eta 00m:00s]
fio-read: (groupid=0, jobs=1): err= 0: pid=16466: Fri Mar  2 18:50:12 2018
   read: IOPS=8138, BW=31.8MiB/s (33.3MB/s)(9537MiB/300001msec)
    slat (nsec): min=411, max=1898.3k, avg=8940.25, stdev=14105.45
    clat (nsec): min=1322, max=105788k, avg=110348.45, stdev=303158.93
     lat (usec): min=58, max=105804, avg=119.95, stdev=303.61
    clat percentiles (usec):
     |  1.00th=[   63],  5.00th=[   70], 10.00th=[   75], 20.00th=[   80],
     | 30.00th=[   85], 40.00th=[   89], 50.00th=[   94], 60.00th=[   99],
     | 70.00th=[  105], 80.00th=[  114], 90.00th=[  133], 95.00th=[  159],
     | 99.00th=[  318], 99.50th=[  515], 99.90th=[ 1336], 99.95th=[ 6783],
     | 99.99th=[10945]
   bw (  KiB/s): min=21112, max=39648, per=100.00%, avg=32556.91, stdev=2820.83, samples=599
   iops        : min= 5278, max= 9912, avg=8139.21, stdev=705.21, samples=599
  lat (usec)   : 2=0.01%, 4=0.01%, 10=0.01%, 20=0.01%, 50=0.05%
  lat (usec)   : 100=62.33%, 250=36.00%, 500=1.09%, 750=0.26%, 1000=0.11%
  lat (msec)   : 2=0.07%, 4=0.01%, 10=0.06%, 20=0.01%, 50=0.01%
  lat (msec)   : 100=0.01%, 250=0.01%
  cpu          : usr=3.90%, sys=11.16%, ctx=2441004, majf=0, minf=39
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=2441454,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
   READ: bw=31.8MiB/s (33.3MB/s), 31.8MiB/s-31.8MiB/s (33.3MB/s-33.3MB/s), io=9537MiB (10.0GB), run=300001-300001msec

Disk stats (read/write):
  vda: ios=2440631/772, merge=0/489, ticks=265889/25698, in_queue=290919, util=88.45%


2. 配置文件

```
fio fio.conf
```

