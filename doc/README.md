
Note: wcdj is a repo that includes useful codes or libs at common work or study (first commit locally from my macbook)


----------------------------------------------------
update: 2014-03-25
1, add tools about glibc, tell 'How to check glibc version'.
2, add ftok.c from glibc codes.


----------------------------------------------------
update: 2014-03-24
1, add tools about sv_shm.
key_t ftok(const char *path, int id);
The ftok function creates key_t like this: proj_id(ff)dev(ff)ino(ffff)


----------------------------------------------------
update: 2014-03-20
1, add modules directory includes fifo module.
There is a simple demo that client using non-block write and svr using block read in demo/fifo dir.


----------------------------------------------------
update: 2014-03-19
1, add modules directory includes http(libcurl) module. 
PS: of course, you may also use [curl] command tool to send http request.


----------------------------------------------------
update: 2014-03-16
1, add tools about fifo dirs that includes four examples of using fifo.
2, update cpu_mem notes about top command usage.


----------------------------------------------------
update: 2014-03-12
1, add tools about inode and hack dirs.


----------------------------------------------------
update: 2014-03-10
1, update Inc.sh to distinguish OS and set it's PROJECT_HOME path.
2, add clear_svmq_linux_suse.sh in tools.


----------------------------------------------------
update: 2014-03-09
1, add clear_svmq_mac.sh tool to clear svmq. the usage is like below:
$./clear_svmq_mac.sh 
svmq info(6): 0x0000276d 0x00002768 0x00002769 0x0000276a 0x0000276b 0x0000276c
clear 0x0000276d SVMQ-keys
clear 0x00002768 SVMQ-keys
clear 0x00002769 SVMQ-keys
clear 0x0000276a SVMQ-keys
clear 0x0000276b SVMQ-keys
clear 0x0000276c SVMQ-keys
clear over
$./clear_svmq_mac.sh 
svmq info(0): 
noting to clear
clear over

Note that if you clear msgkey from system, the server would throw exception, because server used blocking mode, the error information likes below:
ERROR: [pid=822,CServer.cpp:293]receive req from SV-MQ, key[10093] error[msgrcv: msgid[131072] err[90:Identifier removed]]


----------------------------------------------------
update: 2014-03-08
1, add Inc_proxy.sh and safe_wcdj_proxy.sh then you can start any number of proxy you like.
For example, if you set PROXY_NUM (in Inc_proxy.sh) to 2, then the calculation is like below:
client -> (10088) proxy1 (10089) -> (10089) proxy1 (10090) -> server
You can use command [ipcs -q] to check the SV-MQs are created that is right or not.


----------------------------------------------------
update: 2014-03-06
1, add BUILD_DEBUG_PROXY macro:
Note use proxy, you should set correct SV-MQ key in Inc.sh, so that the message can go like below:
client (set CLIENTSVMQKEY[10088]) -> proxy (set CLIENTSVMQKEY[10088] and SERVERSVMQKEY[10089]) -> server (set SERVERSVMQKEY[10089])

PS: performance test result:
Environment, suse32, 15G memory, 1cpu, 4cores
1 client -> 1 proxy -> 1 svr, 15k msgsize, 8w+/s, %CPU=100, so read/write bandwidth is about (15k*8W)/s = 1.2G/s
1 client -> 5 proxy -> 1 svr, 15k msgsize, 35k+/s, %CPU=80, so read/write bandwidth is about (15k*35k)/s = 525M/s


----------------------------------------------------
update: 2014-03-05
1, modify start shell script to support multi-client.
Note: in order to test performance, you should use [./safe_wcdj_client.sh start > /dev/null], do not use printf, etc. system function.
2, add some tools to show top, cpu and /proc/* info.
3, add BLOCK_ACCEPT macro: 
If you set this macro that svr uses block msgrcv to accept request (CPU usage will not high). In the opposite, do not set this macro that svr will accept request in non-block mode (CPU usage is always 100%).
Note that this macro is only used in SERVER_MODE.

PS: performance test result:
Environment, suse32, 15G memory, 1cpu, 4cores
10 clients -> svmq -> 1 svr, 15k msgsize, 10w+/s, %CPU=100, so read/write bandwidth is about (15k*10^5)/s = 1.5GB/s 
10 clients -> svmq -> 1 svr, 32 msgsize, 70w+/s, %CPU=100, so read/write bandwidth is about (32*7*10^5)/s = 22MB/s

System V IPC vs POSIX IPC: 
http://stackoverflow.com/questions/4582968/system-v-ipc-vs-posix-ipc


----------------------------------------------------
update: 2014-03-04
1, add comm libs, includes string-parse, option-parse[like ./prog -para1=value1 -para2=value2 ... dest], so you could transmit paras to prog.


----------------------------------------------------
update: 2014-02-28
1, make tools directory and add SV-MQ monitor script, compare file script


----------------------------------------------------
update: 2014-02-27
1, add SV-MQ including block and non-block interface
note: System V IPC do not belongs to POSIX standard, you can find /usr/include/linux/msg.h and see some about Linux ipc data structure.

You may use command [/sbin/sysctl -a | grep msg] to see current kernel paras, or use below command to see separately:
cat /proc/sys/kernel/msgmnb, this means max size of a message queue.
cat /proc/sys/kernel/msgmax, this means max size of message (bytes).
cat /proc/sys/kernel/msgmni, this means max # of msg queue identifiers.

You may use command [ipcs -q | grep key] to see the status about queue. If you wanna delete the queue you created just now, you may use command [ipcrm -Q key] or [ipcrm -q msqid] to delete it.


----------------------------------------------------
update: 2014-02-22
1, create wcdj repo
2, svr is a daemon
3, add terminal and file log
4, You can check ur program version using below command:
strings ur_prog | grep "Version"
Linux SUSE like this:
WCDJ|VersionV1.0R010_2014/02/22-22:31:01 |gcc_4.1.2 20070115 (prerelease) (SUSE Linux)
OS X like this:
WCDJ|VersionV1.0R010_2014/02/22-22:31:01 |gcc_4.2.1 Compatible Apple LLVM 5.0 (clang-500.2.79)

