# Two-Phase Protocol

* Phase 1

``` 
客户端向所有的服务器请求锁
```

* Phase 2

``` c
if 客户端成功获得了所有服务器的锁
   客户端以可靠的方式向每个服务器发送命令，随即释放锁
else
   客户端释放已经获得的锁
   客户端等待一段时间，再重新进入Phase 1
```

# 朴素的基于票的协议

* Phase 1

```
客户端向所有服务器请求一张票
```

* Phase 2

``` c
if 收到过半数服务器的回复
   客户端将获得的票和命令一起发给每个服务器
   服务器检查票的状态，如果票仍有效，则存储命令并给该客户端一个正反馈消息
else
   客户端等待，并重新进入Phase 1
```

* Phase 3

``` c
if 客户端从过半数服务器得到正反馈
   客户端告诉所有服务器执行之前存储的命令
else
   客户端等待，然后重新进入Phase 1
```

# Paxos

* Init

``` c
客户端：                         
c      // 等待执行的命令         
t = 0; // 当前尝试的票号       

服务器：
T_max = 0;   // 当前已发布的最大票号
C = null;    // 当前存储的命令
T_store = 0; // 用来存储命令C的票
```

* Phase 1

``` c
客户端： 
t = t + 1;
向所有服务器发送消息，请求得到编号为t的票

服务器：
if t > T_max
   T_max = t;
   return ok(T_store, C);
```

* Phase 2

``` c
客户端：
if 过半数服务器回复ok
   选择T_store值最大的(T_store, C)
   if T_store > 0
      c = C;
   向这些回复了ok的服务器发送消息 propose(t, c);

服务器：
if t = T_max 
   C = c;
   T_store = t;
   return success;
```

* Phase 3

``` c
客户端：
if 过半数服务器回复 success
   向每个服务器发送消息 execute(c);
```

## 引理

将客户端发送的一条消息`propose(t, c)`称为一个内容是`(t, c)`的提案。如果一项提案`(t, c)`被存储在过半数服务器上，则称该提案被选中。如果已经存在一个被选中的`propose(t, c)`，则对于后续每一个`propose(t', c')`，`c' = c`将始终成立 `(t' > t)`。

## 定理

如果一条命令`c`被某些服务器执行，那么所有的服务器都最终将执行命令`c`。
